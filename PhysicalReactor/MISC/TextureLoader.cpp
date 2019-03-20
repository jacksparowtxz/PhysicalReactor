#include "stdafx.h"
#define STB_IMAGE_IMPLEMENTATION
#include "TextureLoader.h"

#include "stb_image.h"

#include"Render/ShaderManager.h"
#include"MISC/Mathhelper.h"
using namespace PRE;


TextureLoader::TextureLoader()
{

}


TextureLoader::~TextureLoader()
{

}

void TextureLoader::LoadTexture(const string & TexturefileName, Texture2D* LoadMap,bool UseCubeMap)
{

	 std::string exname=GetExtensionFromFileName(TexturefileName);

	 if (exname=="dds")
	 {
		 const char* filenamechar = TexturefileName.c_str();
		 int iSize;
		 wchar_t* wfilename = nullptr;;
		 iSize = MultiByteToWideChar(CP_ACP, 0, filenamechar, -1, NULL, 0);
		 wfilename = (wchar_t *)malloc(iSize * sizeof(wchar_t));
		 MultiByteToWideChar(CP_ACP, 0, filenamechar, -1, wfilename, iSize);
		 ID3D11Resource* TexResrouce = nullptr;
		 ID3D11ShaderResourceView* srv = nullptr;
		 HRESULT hr= DirectX::CreateDDSTextureFromFile((ID3D11Device*)Renderer::GetDevice()->GetDevice(), wfilename, &TexResrouce,
			&srv);
		 LoadMap->SRV=(CPUHandle)srv;
		 SUCCEEDED(hr);
		 SAFE_RELEASE(TexResrouce);
		 free(wfilename);
	 }
	 else if (exname == "hdr" || exname == "hdri")
	 {
		 const char* szname = TexturefileName.c_str();
		 int width = 0;
		 int height = 0;
		 int channelcount = 0;
		 unsigned char* data = stbi_load(szname, &width, &height, &channelcount, 4);
		 Texture2D* EnvTexture = new Texture2D;
		 TextureDesc desc;
		 desc.ArraySize = 1;
		 desc.BindFlags = BIND_SHADER_RESOURCE | BIND_UNORDERED_ACCESS;
		 desc.CPUAccessFlags = 0;
		 desc.Format = FORMAT_R32G32B32A32_FLOAT;
		 desc.Height = static_cast<uint32_t>(height);
		 desc.Width = static_cast<uint32_t>(width);
		 desc.MipLevels = (UINT)log2(max(width, height));
		 desc.MiscFlags =0;
		 desc.Usage = USAGE_DEFAULT;

		 UINT mipwidth = width;
		 SubresourceData* InitData = new SubresourceData[desc.MipLevels];
		 for (UINT mip = 0; mip < desc.MipLevels; ++mip)
		 {
			 InitData[mip].pSysMem = data;
			 InitData[mip].SysMemPitch = static_cast<UINT>(mipwidth*(channelcount));
			 mipwidth = max(1, mipwidth / 2);
		 }
		 EnvTexture->RequestIndepentShaderReourcesForMIPs(true);
		 EnvTexture->RequesIndenpentUnorderedAccessResoucesForMips(true);
		 HRESULT hr = Renderer::GetDevice()->CreateTexture2D(&desc, InitData, &EnvTexture);
		 assert(SUCCEEDED(hr));
		 stbi_image_free(data);
		 delete InitData;
		 
		
		 if (UseCubeMap)
		 {
			 TextureDesc CubeMapdesc;
			 CubeMapdesc.ArraySize = 6;
			 CubeMapdesc.BindFlags = BIND_SHADER_RESOURCE | BIND_UNORDERED_ACCESS|BIND_RENDER_TARGET;
			 CubeMapdesc.CPUAccessFlags = 0;
			 CubeMapdesc.Format = FORMAT_R16G16B16A16_FLOAT;
			 CubeMapdesc.Height = static_cast<uint32_t>(1024);
			 CubeMapdesc.Width = static_cast<uint32_t>(1024);
			 CubeMapdesc.MipLevels =0;
			 CubeMapdesc.MiscFlags = RESOURCE_MISC_TEXTURECUBE|RESOURCE_MISC_GENERATE_MIPS;
			 CubeMapdesc.Usage = USAGE_DEFAULT;
			 HRESULT chr = Renderer::GetDevice()->CreateTexture2D(&CubeMapdesc, nullptr, &LoadMap);
			 assert(SUCCEEDED(chr));
			 ComputerPSO CSPSO;
			 CSPSO.desc.cs = Renderer::shadermanager->GetComputerShader("equirect2cube.hlsl");

			 GPUResource NullResource;

			 Sampler Computersampler;
			 SamplerDesc computerdesc;

			 computerdesc.Filter = FILTER_MIN_MAG_MIP_LINEAR;
			 computerdesc.AddressU = TEXTURE_ADDRESS_WRAP;
			 computerdesc.AddressV = TEXTURE_ADDRESS_WRAP;
			 computerdesc.AddressW = TEXTURE_ADDRESS_WRAP;
			 computerdesc.MaxAnisotropy = 1;
			 computerdesc.MinLOD = 0;
			 computerdesc.MaxLOD = FLT_MAX;

			 Renderer::GetDevice()->CreateSamplerState(&computerdesc,&Computersampler);

			 Renderer::GetDevice()->BindResource(CS_STAGE, EnvTexture, 0);
			 Renderer::GetDevice()->BindUAV(CS_STAGE, LoadMap, 0);
			 Renderer::GetDevice()->BindSampler(CS_STAGE, &Computersampler, 0, 1);
			 Renderer::GetDevice()->BindComputerPSO(&CSPSO);
			 Renderer::GetDevice()->Dispatch(LoadMap->GetDesc().Width / 32, LoadMap->GetDesc().Height / 32, 6);
			 Renderer::GetDevice()->BindUAV(CS_STAGE, &NullResource, 0);
			 Renderer::GetDevice()->GenerateMips(LoadMap);
			 delete EnvTexture;
		 }
		 else
		 {
			 LoadMap = std::move(EnvTexture);
		 }
	 }
	 else
	 {
		 const char* szname = TexturefileName.c_str();
		 int width = 0;
		 int height = 0;
		 int channelcount = 0;
		 unsigned char* data = stbi_load(szname, &width, &height, &channelcount, 4);

		 TextureDesc desc;
		 desc.ArraySize = 1;
		 desc.BindFlags = BIND_SHADER_RESOURCE | BIND_UNORDERED_ACCESS;
		 desc.CPUAccessFlags = 0;
		 desc.Format = FORMAT_R8G8B8A8_UNORM;
		 desc.Height = static_cast<uint32_t>(height);
		 desc.Width = static_cast<uint32_t>(width);
		 desc.MipLevels = (UINT)log2(max(width, height));
		 desc.MiscFlags = 0;
		 desc.Usage = USAGE_DEFAULT;

		 UINT mipwidth = width;
		 SubresourceData* InitData = new SubresourceData[desc.MipLevels];
		 for (UINT mip = 0; mip < desc.MipLevels; ++mip)
		 {
			 InitData[mip].pSysMem = data;
			 InitData[mip].SysMemPitch = static_cast<UINT>(mipwidth*(channelcount));
			 mipwidth = max(1, mipwidth / 2);
		 }
		 LoadMap->RequestIndepentShaderReourcesForMIPs(true);
		 LoadMap->RequesIndenpentUnorderedAccessResoucesForMips(true);
		 HRESULT hr = Renderer::GetDevice()->CreateTexture2D(&desc, InitData, &LoadMap);
		 assert(SUCCEEDED(hr));
		 stbi_image_free(data);
		 delete InitData;
	 }
}

void TextureLoader::MakeRadianceMap(Texture2D* ufilterEnvmap,Texture2D* env_Map,Texture2D* Splut)
{
	ComputerPSO CSPSO;
	CSPSO.desc.cs = Renderer::shadermanager->GetComputerShader("PerfilterEnvMap.hlsl");
	struct SpeularMapFilterSetting
	{
		float roughness;
		XMFLOAT3 Padding;
	};
	GPUResource NullResource;
	GPUBufferDesc smpdesc;
	smpdesc.BindFlags = BIND_CONSTANT_BUFFER;
	smpdesc.ByteWidth = sizeof(SpeularMapFilterSetting);
	smpdesc.CPUAccessFlags = 0;
	smpdesc.MiscFlags = 0;
	smpdesc.Usage = USAGE_DEFAULT;
	smpdesc.StructureByteStride = 0;

	GPUBuffer* SpeularMapFilterSettingCB = new GPUBuffer;

	Renderer::GetDevice()->CreateBuffer(&smpdesc,nullptr, SpeularMapFilterSettingCB);


	TextureDesc CubeMapdesc;
	CubeMapdesc.ArraySize = 6;
	CubeMapdesc.BindFlags = BIND_SHADER_RESOURCE | BIND_UNORDERED_ACCESS|BIND_RENDER_TARGET;
	CubeMapdesc.CPUAccessFlags = 0;
	CubeMapdesc.Format = FORMAT_R16G16B16A16_FLOAT;
	CubeMapdesc.Height = static_cast<uint32_t>(1024);
	CubeMapdesc.Width = static_cast<uint32_t>(1024);
	CubeMapdesc.MipLevels = 0;
	CubeMapdesc.MiscFlags = RESOURCE_MISC_TEXTURECUBE|RESOURCE_MISC_GENERATE_MIPS;
	CubeMapdesc.Usage = USAGE_DEFAULT;

	Renderer::GetDevice()->CreateTexture2D(&CubeMapdesc,nullptr,&env_Map);

	for (int arraySlice = 0; arraySlice < 6; ++arraySlice)
	{
		Renderer::GetDevice()->CopyTexture2D_Region(env_Map,0,0,0, ufilterEnvmap,0,arraySlice);
	}
	Sampler Computersampler;
	SamplerDesc computerdesc;
	computerdesc.Filter = FILTER_MIN_MAG_MIP_LINEAR;
	computerdesc.AddressU = TEXTURE_ADDRESS_WRAP;
	computerdesc.AddressV = TEXTURE_ADDRESS_WRAP;
	computerdesc.AddressW = TEXTURE_ADDRESS_WRAP;
	computerdesc.MaxAnisotropy = 1;
	computerdesc.MinLOD = 0;
	computerdesc.MaxLOD = FLT_MAX;
	Renderer::GetDevice()->CreateSamplerState(&computerdesc, &Computersampler);
	Renderer::GetDevice()->BindComputerPSO(&CSPSO);
	Renderer::GetDevice()->BindResource(CS_STAGE,ufilterEnvmap,0);
	Renderer::GetDevice()->BindSampler(CS_STAGE, &Computersampler, 0,1);
	

	const float deltaRoughness = 1.0f / PRE::fmax(float(env_Map->GetDesc().MipLevels - 1), 1.0f);
	for (UINT level = 1, size = 512; level < env_Map->GetDesc().MipLevels; ++level, size /= 2)
	{
		const UINT numGroups = MathHelper::Max<int>(1, size / 32);
		const SpeularMapFilterSetting spmfs = { level*deltaRoughness };
		Renderer::GetDevice()->UpdateBuffer(SpeularMapFilterSettingCB,&spmfs);
		Renderer::GetDevice()->BindConstantBuffer(CS_STAGE,SpeularMapFilterSettingCB,0,nullptr,nullptr);
		Renderer::GetDevice()->BindUAV(CS_STAGE,env_Map,0);
		Renderer::GetDevice()->Dispatch(numGroups, numGroups, 6);
	}
	Renderer::GetDevice()->BindConstantBuffer(CS_STAGE, nullptr, 0, NULL, NULL);
	Renderer::GetDevice()->BindUAV(CS_STAGE, &NullResource, 0);


	ComputerPSO ComputeSpLut;
	ComputeSpLut.desc.cs = Renderer::shadermanager->GetComputerShader("SpLUT.hlsl");

	TextureDesc sptexturedesc;
	sptexturedesc.Format = FORMAT_R16G16_FLOAT;
	sptexturedesc.Width = 256;
	sptexturedesc.Height = 256;
	sptexturedesc.MipLevels = 1;
	sptexturedesc.BindFlags = BIND_SHADER_RESOURCE | BIND_UNORDERED_ACCESS;
	sptexturedesc.Usage = USAGE_DEFAULT;
	sptexturedesc.ArraySize = 1;
	sptexturedesc.SampleDesc.Count = 1;
	
	Renderer::GetDevice()->CreateTexture2D(&sptexturedesc,nullptr,&Splut);


	Renderer::GetDevice()->BindResource(CS_STAGE, Splut, 0);
	Renderer::GetDevice()->BindComputerPSO(&ComputeSpLut);
	Renderer::GetDevice()->Dispatch(Splut->desc.Width/32, Splut->desc.Height / 32,1);

	
	
}

void TextureLoader::MakeIadiacneMap(shcoeffs* cofs)
{
	///////////USE spherical_harmonics
	PRE::LoadSH("sh_coefficients.txt",cofs);

}

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

void TextureLoader::LoadTexture(const string & TexturefileName, Texture2D* LoadMap,bool UseCubeMap, bool srgb /*= false*/,UINT levels /*= 0*/)
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
		 float* data = stbi_loadf(szname, &width, &height, &channelcount, 4);
		 Texture2D* EnvTexture = new Texture2D;
		 TextureDesc desc;
		 desc.ArraySize = 1;
		 desc.BindFlags = BIND_SHADER_RESOURCE;
		 desc.CPUAccessFlags = 0;
		 desc.Format = FORMAT_R32G32B32A32_FLOAT;
		 desc.Height = static_cast<UINT>(height);
		 desc.Width = static_cast<UINT>(width);
		 desc.MipLevels = 1;
		 desc.MiscFlags =0;
		 desc.Usage = USAGE_DEFAULT;
		 desc.SampleDesc.Count = 1;
		 desc.SampleDesc.Quality = 0;
		 UINT mipwidth = width;
		 SubresourceData dataDesc = {};
		 SubresourceData* pDataDesc = nullptr;
		
		 dataDesc.pSysMem = data;
		 dataDesc.SysMemPitch = mipwidth*16;
		 pDataDesc = &dataDesc;
		 EnvTexture->RequestIndepentShaderReourcesForMIPs(false);
		 EnvTexture->RequesIndenpentUnorderedAccessResoucesForMips(false);
		 HRESULT hr;
		 if (UseCubeMap)
		 {
			hr = Renderer::GetDevice()->CreateTexture2D(&desc, pDataDesc, &EnvTexture);
#ifdef PREDEBUG
			Renderer::GetDevice()->SetName(EnvTexture, "EnvTexture");
#endif // DEBUG
		 }
		 else
		 {
			 hr = Renderer::GetDevice()->CreateTexture2D(&desc, pDataDesc, &LoadMap);
#ifdef PREDEBUG
			 Renderer::GetDevice()->SetName(LoadMap, "LoadMap1");
#endif // DEBUG
			 delete EnvTexture;
		 }
		
		 assert(SUCCEEDED(hr));
		 stbi_image_free(data);
		
		 
		
		 if (UseCubeMap)
		 {
			 TextureDesc CubeMapdesc;
			 CubeMapdesc.ArraySize = 6;
			 CubeMapdesc.BindFlags = BIND_SHADER_RESOURCE | BIND_UNORDERED_ACCESS|BIND_RENDER_TARGET;
			 CubeMapdesc.CPUAccessFlags = 0;
			 CubeMapdesc.Format = FORMAT_R16G16B16A16_FLOAT;
			 CubeMapdesc.Height = static_cast<UINT>(1024);
			 CubeMapdesc.Width = static_cast<UINT>(1024);
			 CubeMapdesc.MipLevels =0;
			 CubeMapdesc.MiscFlags = RESOURCE_MISC_TEXTURECUBE|RESOURCE_MISC_GENERATE_MIPS;
			 CubeMapdesc.Usage = USAGE_DEFAULT;
			 CubeMapdesc.SampleDesc.Count = 1;
			 CubeMapdesc.SampleDesc.Quality = 0;
			 HRESULT chr = Renderer::GetDevice()->CreateTexture2D(&CubeMapdesc, nullptr, &LoadMap);
#ifdef PREDEBUG
			 Renderer::GetDevice()->SetName(LoadMap, "LoadMap2");
#endif // DEBUG
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
#ifdef PREDEBUG
			 Renderer::GetDevice()->SetName(&Computersampler,"Computersampler");
#endif // DEBUG
			 Renderer::GetDevice()->BindResource_Immediate(CS_STAGE, EnvTexture, 0);
			 Renderer::GetDevice()->BindUAV_Immediate(CS_STAGE, LoadMap, 0);
			 Renderer::GetDevice()->BindSampler_Immediate(CS_STAGE, &Computersampler, 0, 1);
			 Renderer::GetDevice()->BindComputerPSO_Immediate(&CSPSO);
			 Renderer::GetDevice()->Dispatch_Immediate(LoadMap->GetDesc().Width / 32, LoadMap->GetDesc().Height / 32, 6);
			 Renderer::GetDevice()->BindUAV_Immediate(CS_STAGE, &NullResource, 0);
			 Renderer::GetDevice()->GenerateMips_Immediate(LoadMap);
			 delete EnvTexture;
		 }
	 }
	 else
	 {
		 const char* szname = TexturefileName.c_str();
		 int width;
		 int height;
		 int comp;
		 const int channelcount = 4;
		 unsigned char* data = stbi_load(szname, &width, &height, &comp, channelcount);
		 
		 TextureDesc desc;
		 desc.ArraySize = 1;
		 desc.BindFlags = BIND_SHADER_RESOURCE;
		 if (levels == 0 )
		 {
			 desc.BindFlags |= BIND_RENDER_TARGET;
			 desc.MiscFlags = RESOURCE_MISC_GENERATE_MIPS;
		 }
		 if (!srgb)
		 {
			 desc.BindFlags |= BIND_UNORDERED_ACCESS;
		 }
		 desc.CPUAccessFlags = 0;
		 desc.Format = srgb?FORMAT_R8G8B8A8_UNORM_SRGB: FORMAT_R8G8B8A8_UNORM;
		 desc.Height = static_cast<uint32_t>(height);
		 desc.Width = static_cast<uint32_t>(width);
		 desc.MipLevels = (levels== 0) ? PRE::numMipmapLevels(width, height) : levels;
		 desc.SampleDesc.Count = 1;
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
		 if (levels == 0)
		 {
			 Renderer::GetDevice()->GenerateMips_Immediate(LoadMap);
		 }
		
#ifdef PREDEBUG
		 Renderer::GetDevice()->SetName(LoadMap, TexturefileName);
#endif // DEBUG
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
	CubeMapdesc.SampleDesc.Count = 1;
	CubeMapdesc.SampleDesc.Quality = 0;
	env_Map->RequesIndenpentUnorderedAccessResoucesForMips(true);
	env_Map->RequestIndepentShaderReourcesForMIPs(true);
	env_Map->RequestIndepentShaderResourceArraySlices(true);
	Renderer::GetDevice()->CreateTexture2D(&CubeMapdesc,nullptr,&env_Map);

	for (int arraySlice = 0; arraySlice < 6; ++arraySlice)
	{
		Renderer::GetDevice()->CopyTexture2D_Region_Immediate(env_Map,0,0,0, ufilterEnvmap,0,arraySlice);
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
	Renderer::GetDevice()->BindComputerPSO_Immediate(&CSPSO);
	Renderer::GetDevice()->BindResource_Immediate(CS_STAGE,ufilterEnvmap,0);
	Renderer::GetDevice()->BindSampler_Immediate(CS_STAGE, &Computersampler, 0,1);
	
	int texturelevel = PRE::numMipmapLevels(env_Map->GetDesc().Width, env_Map->GetDesc().Height)-1;
	const float deltaRoughness = 1.0f / PRE::fmax(float(texturelevel), 1.0f);
	for (UINT level = 1, size = 512; level < texturelevel; ++level, size /= 2)
	{
		const UINT numGroups = MathHelper::Max<UINT>(1, size / 32);
		const SpeularMapFilterSetting spmfs = { level*deltaRoughness };
		Renderer::GetDevice()->UpdateBuffer_Immediate(SpeularMapFilterSettingCB,&spmfs);
		Renderer::GetDevice()->BindConstantBuffer_Immediate(CS_STAGE,SpeularMapFilterSettingCB,0,nullptr,nullptr);
		Renderer::GetDevice()->BindUAV_Immediate(CS_STAGE,env_Map,0, level);
		Renderer::GetDevice()->Dispatch_Immediate(numGroups, numGroups, 6);
	}
	Renderer::GetDevice()->BindConstantBuffer_Immediate(CS_STAGE, nullptr, 0, NULL, NULL);
	Renderer::GetDevice()->BindUAV_Immediate(CS_STAGE, &NullResource, 0);


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


	Renderer::GetDevice()->BindUAV_Immediate(CS_STAGE, Splut, 0);
	Renderer::GetDevice()->BindComputerPSO_Immediate(&ComputeSpLut);
	Renderer::GetDevice()->Dispatch_Immediate(Splut->desc.Width/32, Splut->desc.Height / 32,1);
	Renderer::GetDevice()->BindUAV_Immediate(CS_STAGE, &NullResource, 0);
	
	
}

void TextureLoader::MakeIadiacneMap(shcoeffs* cofs, Texture2D* Env_map, Texture2D* Iradiacnemap)
{
	///////////USE spherical_harmonics
//	PRE::LoadSH("sh_coefficient.txt",cofs);

	TextureDesc CubeMapdesc;
	CubeMapdesc.ArraySize = 6;
	CubeMapdesc.BindFlags = BIND_SHADER_RESOURCE | BIND_UNORDERED_ACCESS;
	CubeMapdesc.CPUAccessFlags = 0;
	CubeMapdesc.Format = FORMAT_R16G16B16A16_FLOAT;
	CubeMapdesc.Height = static_cast<uint32_t>(32);
	CubeMapdesc.Width = static_cast<uint32_t>(32);
	CubeMapdesc.MipLevels = 1;
	CubeMapdesc.MiscFlags = RESOURCE_MISC_TEXTURECUBE;
	CubeMapdesc.Usage = USAGE_DEFAULT;
	CubeMapdesc.SampleDesc.Count = 1;
	CubeMapdesc.SampleDesc.Quality = 0;
	Iradiacnemap->RequesIndenpentUnorderedAccessResoucesForMips(true);
	Renderer::GetDevice()->CreateTexture2D(&CubeMapdesc, nullptr, &Iradiacnemap);

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


	Renderer::GetDevice()->BindResource_Immediate(CS_STAGE,Env_map,0);
	Renderer::GetDevice()->BindSampler_Immediate(CS_STAGE,&Computersampler,0,1);
	Renderer::GetDevice()->BindUAV_Immediate(CS_STAGE, Iradiacnemap,0);

	ComputerPSO ComputeIrad;
	ComputeIrad.desc.cs = Renderer::shadermanager->GetComputerShader("IraMap.hlsl");
	Renderer::GetDevice()->BindComputerPSO_Immediate(&ComputeIrad);

	Renderer::GetDevice()->Dispatch_Immediate(1,1,6);
	Renderer::GetDevice()->BindComputerPSO_Immediate(nullptr);


}

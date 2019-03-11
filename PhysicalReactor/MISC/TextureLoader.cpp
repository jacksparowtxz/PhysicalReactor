#include "stdafx.h"
#define STB_IMAGE_IMPLEMENTATION
#include "TextureLoader.h"

#include "stb_image.h"

#include"Render/ShaderManager.h"
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
		 LoadMap = std::move(EnvTexture);
		
		 if (UseCubeMap)
		 {
			 TextureDesc CubeMapdesc;
			 CubeMapdesc.ArraySize = 6;
			 CubeMapdesc.BindFlags = BIND_SHADER_RESOURCE | BIND_UNORDERED_ACCESS;
			 CubeMapdesc.CPUAccessFlags = 0;
			 CubeMapdesc.Format = FORMAT_R16G16B16A16_FLOAT;
			 CubeMapdesc.Height = static_cast<uint32_t>(1024);
			 CubeMapdesc.Width = static_cast<uint32_t>(1024);
			 CubeMapdesc.MipLevels = (UINT)log2(max(1024, 1024));
			 CubeMapdesc.MiscFlags = RESOURCE_MISC_TEXTURECUBE;
			 CubeMapdesc.Usage = USAGE_DEFAULT;
			 HRESULT chr = Renderer::GetDevice()->CreateTexture2D(&CubeMapdesc, nullptr, &LoadMap);
			 assert(SUCCEEDED(chr));
			 ComputerPSO CSPSO;
			 CSPSO.desc.cs == Renderer::shadermanager->GetComputerShader("equirect2cube.hlsl");

			 Sampler Computersampler;
			 Computersampler.desc.Filter = FILTER_MIN_MAG_MIP_LINEAR;
			 Computersampler.desc.AddressU = TEXTURE_ADDRESS_WRAP;
			 Computersampler.desc.AddressV = TEXTURE_ADDRESS_WRAP;
			 Computersampler.desc.AddressW = TEXTURE_ADDRESS_WRAP;
			 Computersampler.desc.MaxAnisotropy = 1;
			 Computersampler.desc.MinLOD = 0;
			 Computersampler.desc.MaxLOD = FLT_MAX;

			 Renderer::GetDevice()->BindResource(CS_STAGE, EnvTexture, 0);
			 Renderer::GetDevice()->BindUAV(CS_STAGE, LoadMap, 0);
			 Renderer::GetDevice()->BindSampler(CS_STAGE, &Computersampler, 0, 1);
			 Renderer::GetDevice()->BindComputerPSO(&CSPSO);
			 Renderer::GetDevice()->Dispatch(LoadMap->GetDesc().Width / 32, LoadMap->GetDesc().Height / 32, 6);
			 Renderer::GetDevice()->GenerateMips(LoadMap);
			 delete EnvTexture;
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

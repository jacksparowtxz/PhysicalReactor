#include "stdafx.h"
#define STB_IMAGE_IMPLEMENTATION
#include "TextureLoader.h"

#include "stb_image.h"
using namespace PRE;


TextureLoader::TextureLoader()
{

}


TextureLoader::~TextureLoader()
{

}

void TextureLoader::LoadTexture(const string & TexturefileName, Texture2D* LoadMap)
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

		 TextureDesc desc;
		 desc.ArraySize = 1;
		 desc.BindFlags = BIND_SHADER_RESOURCE | BIND_UNORDERED_ACCESS;
		 desc.CPUAccessFlags = 0;
		 desc.Format = FORMAT_R32G32B32A32_FLOAT;
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

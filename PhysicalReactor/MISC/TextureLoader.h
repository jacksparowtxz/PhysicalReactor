#pragma once
#include"MISC/DDSTextureLoader.h"
#include"Render/Renderer.h"
#include "CommonInclude.h"
#include"MISC/MiscHelper.h"
class TextureLoader
{
public:
	TextureLoader();
	~TextureLoader();
	void LoadTexture(const string & TexturefileName,Texture2D* LoadMap);
};


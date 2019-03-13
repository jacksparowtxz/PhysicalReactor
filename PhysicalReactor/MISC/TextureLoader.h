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
	void LoadTexture(const string & TexturefileName,Texture2D* LoadMap,bool UseCubeMap);
	void MakeRadianceMap(Texture2D* ufilterEnvmap, Texture2D* Env_map, Texture2D* Splut);
	void MakeIadiacneMap(shcoeffs* cofs);
};


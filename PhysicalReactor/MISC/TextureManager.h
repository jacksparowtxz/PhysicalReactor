#pragma once
#include"MISC/TextureLoader.h"

class TextureManager
{
public:
	static TextureLoader* TextureImport;
	static TextureLoader*  GetLoader()
	{
		if (TextureImport != nullptr)
		{
			return TextureImport;
		}
		return nullptr;
	}
};


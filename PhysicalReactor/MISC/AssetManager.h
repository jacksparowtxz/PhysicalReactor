#pragma once
#include"MISC/MeshLoad.h"

using namespace PRE;

class AssetManager
{
public:
	static GameMeshImport* MeshImport;
	static GameMeshImport*  GetDevice()
	{
		if (MeshImport!= nullptr)
		{
			return MeshImport;
		}
		return nullptr;
	}
	
};


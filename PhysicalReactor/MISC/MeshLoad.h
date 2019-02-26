#pragma once
#include "Render/StaticMesh.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include <vector>


#include "assimp/pbrmaterial.h"
#include "MISC/Frustum.h"
#include "Render/Renderer.h"
#include "Render/RenderDevice_DX11.h"
#include <tchar.h>
#include <windows.h>

#include "Render/GraphicsResource.h"


using namespace PRE;

class GameMeshImport
{
public:
	GameMeshImport();
	~GameMeshImport();
	void Import(std::string pFile,StaticMesh* loadmesh);
	
private:
	void ProcessNode(aiNode* node, const aiScene* scene, StaticMesh* loadmesh);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene, StaticMesh* loadmesh);
	vector<Texture2D*> loadMaterialTexture(aiMaterial* mat, aiTextureType type, std::string name);
	//Texture2D  TextureFormFile(const char* path, std::string directory);
	std::string directory;
	static std::string lastname;
	float MaxX, MaxY, MaxZ, MinX, MinY, MinZ;

	
	//////////////GLTF_SPECTIAL
	std::string GetPathAssimp(aiTextureType text_type, aiMaterial* ai_material);
	std::string GetPathAssimp(std::string, aiTextureType type, aiMaterial* ai_material);
	XMFLOAT3 GetVectorFactorAssimp(std::string key, unsigned int type, unsigned int slot, aiMaterial* ai_material, XMFLOAT3 default_value = {0,0,0});
	float GetNumerFactorAssimp(std::string key, unsigned int type, unsigned int slot, aiMaterial* ai_material, float defalut_value=0.0f);
};


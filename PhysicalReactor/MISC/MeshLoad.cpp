#include "MeshLoad.h"


using namespace PRE;

std::string  GameMeshImport::lastname = "";
GameMeshImport::GameMeshImport()
{
	MaxX = MaxY = MaxZ = MinX = MinY = MinZ = 0.f;
}

GameMeshImport::~GameMeshImport()
{

}

void GameMeshImport::Import(std::string pFile, StaticMesh* loadmesh)
{

	Assimp::Importer importer;
	const aiScene* pScene = importer.ReadFile(pFile.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_ConvertToLeftHanded);
	if (pScene)
	{
		directory = pFile.substr(0, pFile.find_last_of('/'));
		ProcessNode(pScene->mRootNode, pScene,loadmesh);
		XMFLOAT3 max = { MaxX,MaxY,MaxZ };
		XMFLOAT3 min = { MinX,MinY,MinZ };
		AABB*aabb=new AABB;
		aabb->create(max, min);
		loadmesh->aabb = aabb;
	}
	MaxX = 0.0f; 
	MaxY = 0.0f;
	MaxZ = 0.0f;
	MinX = 0.0f;
	MinY = 0.0f;
	MinZ = 0.0f;
}

void GameMeshImport::ProcessNode(aiNode* node, const aiScene* scene, StaticMesh* loadmesh)
{
	for (uint32_t i=0;i<node->mNumMeshes;i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene, loadmesh);
	}
	for (uint32_t i=0;i<node->mNumChildren;i++)
	{
		ProcessNode(node->mChildren[i], scene,loadmesh);
	}
}

void GameMeshImport::ProcessMesh(aiMesh* mesh, const aiScene* scene, StaticMesh *loadmesh)
{
	vector<Vertex> Vertices;
	vector<UINT> indices;

	for (uint32_t i=0;i<mesh->mNumVertices;i++)
	{
		Vertex vertex;
		XMFLOAT3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.Pos = vector;

		MaxX = (vector.x > MaxX) ? vector.x : MaxX;
		MaxY = (vector.y > MaxY) ? vector.y : MaxY;
		MaxZ = (vector.z > MaxZ) ? vector.z : MaxZ;

		MinX = (vector.x < MinX) ? vector.x : MinX;
		MinY = (vector.y < MinY) ? vector.y : MinY;
		MinZ = (vector.z < MinZ) ? vector.z : MinZ;



		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.Normal = vector;

		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.Tangent = vector;
		
		if (mesh->mTextureCoords[0])
		{
			XMFLOAT2 uv;
			uv.x = mesh->mTextureCoords[0][i].x;
			uv.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoord = XMFLOAT2(uv.x, uv.y);
		}
		else
		{
			vertex.TexCoord = { 0.0f, 0.0f};
		}
		Vertices.push_back(std::move(vertex));
	}
	for (uint32_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (uint32_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(std::move(face.mIndices[j]));
		}
	}
	Material* meshmaterial = new Material();
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		vector<Texture2D*> Maps;
	
		SamplerDesc InitSamplerDesc;
		InitSamplerDesc.Filter = FILTER_ANISOTROPIC;
		InitSamplerDesc.AddressU = TEXTURE_ADDRESS_WRAP;
		InitSamplerDesc.AddressV = TEXTURE_ADDRESS_WRAP;
		InitSamplerDesc.AddressW = TEXTURE_ADDRESS_WRAP;
		InitSamplerDesc.ComparsionFunc = COMPARSION_NEVER;
		InitSamplerDesc.MaxAnisotropy = 16;

		Maps = loadMaterialTexture(material, aiTextureType_DIFFUSE, "texture_diffuse",true);
		if (Maps.size() > 0)
		{
			meshmaterial->BaseColorMap = std::move(Maps[0]);
			meshmaterial->BaseColorMapName = std::move(string("texture_diffuse"));
			meshmaterial->SetBaseColorSampler(InitSamplerDesc);
		}

		Maps = loadMaterialTexture(material, aiTextureType_SPECULAR, "texture_specular");
		if (Maps.size() > 0)
		{
			meshmaterial->SpecularMap = std::move(Maps[0]);
			meshmaterial->SpecularMapName = std::move(string("texture_specular"));
			meshmaterial->SetSpecularSampler(InitSamplerDesc);
		}


		Maps = loadMaterialTexture(material, aiTextureType_NORMALS, "texture_normal");
		if (Maps.size() > 0)
		{
			meshmaterial->NormalMap = std::move(Maps[0]);
			meshmaterial->NormalMapName = std::move(string("texture_normal"));
			meshmaterial->SetNormalSampler(InitSamplerDesc);
		}

		Maps = loadMaterialTexture(material, aiTextureType_EMISSIVE, "texture_emissive");
		if (Maps.size() > 0)
		{
			meshmaterial->EmissiveMap = std::move(Maps[0]);
			meshmaterial->EmissiveMapName = std::move(string("texture_emissive"));
			meshmaterial->SetEmissiveSampler(InitSamplerDesc);
		}
		Maps = loadMaterialTexture(material, aiTextureType_OPACITY, "texture_opacity");
		if (Maps.size() > 0)
		{
			meshmaterial->OpacityMap = std::move(Maps[0]);
			meshmaterial->OpacityMapName = std::move(string("texture_opacity"));
			meshmaterial->SetOpacitySampler(InitSamplerDesc);
		}
	
		Maps = loadMaterialTexture(material, aiTextureType_DISPLACEMENT, "texture_displacement");
		if (Maps.size() > 0)
		{
			meshmaterial->WorldDisplacement = std::move(Maps[0]);
			meshmaterial->WorldDisplacementName = std::move(string("texture_displacement"));
			meshmaterial->SetWorldDisplacementSampler(InitSamplerDesc);
		}

		Maps = loadMaterialTexture(material, aiTextureType_REFLECTION, "texture_reflection");
		if (Maps.size() > 0)
		{
			meshmaterial->RefractionMap = std::move(Maps[0]);
			meshmaterial->RefractionMapname = std::move(string("texture_reflection"));
			meshmaterial->SetRefractionSampler(InitSamplerDesc);
		}

		Maps = loadMaterialTexture(material, aiTextureType_HEIGHT, "texture_height");
		{
		
		}

		Maps = loadMaterialTexture(material, aiTextureType_UNKNOWN, "texture_unkonw");
		if (Maps.size() > 0)
		{
			meshmaterial->MetalicMap = std::move(Maps[0]);
			meshmaterial->MetalicMapName = std::move(string("texture_metalic"));
			meshmaterial->SetMetalicSampler(InitSamplerDesc);
		}

		Maps = loadMaterialTexture(material, aiTextureType_LIGHTMAP, "texture_ambient");
		if (Maps.size() > 0)
		{
			meshmaterial->AmbientMap = std::move(Maps[0]);
			meshmaterial->AmbientMapname = std::move(string("texture_Ambient"));
			meshmaterial->SetAmbientSampler(InitSamplerDesc);
		}

		XMFLOAT3 basefactor = GetVectorFactorAssimp(AI_MATKEY_COLOR_DIFFUSE, material);
		float emissive_factor = GetNumerFactorAssimp(AI_MATKEY_COLOR_EMISSIVE, material);
		float metallic_factor = GetNumerFactorAssimp(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLIC_FACTOR, material);
		float rougness_factor = GetNumerFactorAssimp(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_ROUGHNESS_FACTOR, material);
		
		meshmaterial->Metalness = std::move(metallic_factor);
		meshmaterial->Roughness = std::move(rougness_factor);
		meshmaterial->BaseColor = std::move(basefactor);
		meshmaterial->Emssive = std::move(emissive_factor);

	}
	SubMesh *loadsub =new SubMesh(std::move(Vertices),std::move(indices));
	loadsub->material = meshmaterial;
	loadmesh->Meshs.push_back(loadsub);
}

std::vector<Texture2D*> GameMeshImport::loadMaterialTexture(aiMaterial* mat, aiTextureType type, std::string name,bool SRGB /*= false*/)
{
	vector<Texture2D*> textures;
	for (uint32_t i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		Texture2D* texture2d =new Texture2D;

       std::string filename = string(str.C_Str());
	   if (lastname != filename)
	   {
		   filename = directory + "/" + filename;
		   std::string name = std::string(filename.begin(), filename.end());
		   const char* szname = name.c_str();
		   TextureManager::GetLoader()->LoadTexture(name, texture2d,false,SRGB);
		   textures.push_back(texture2d);
		   lastname = std::move(str.C_Str());
		 
	   }
    }
		return textures;
}

std::string GameMeshImport::GetPathAssimp(aiTextureType text_type, aiMaterial* ai_material)
{
	if (ai_material->GetTextureCount(text_type)>0)
	{
		aiString ai_filename;
		std::string filename;

		if (ai_material->GetTexture(text_type,0,&ai_filename)==AI_SUCCESS)
		{
			return ai_filename.C_Str();
		}
	}
	return "";
}

std::string GameMeshImport::GetPathAssimp(std::string key, aiTextureType type, aiMaterial* ai_material)
{
	aiString ai_filename;

	if (ai_material->Get(key.c_str(), type, 0, ai_filename) == AI_SUCCESS)
		return ai_filename.C_Str();
	else
		return "";
}

DirectX::XMFLOAT3 GameMeshImport::GetVectorFactorAssimp(std::string key, unsigned int type, unsigned int slot, aiMaterial* ai_material, XMFLOAT3 default_value /*= { 0,0,0,0 }*/)
{
	aiColor4D ai_color;

	if (ai_material->Get(key.c_str(), type, slot, ai_color) == AI_SUCCESS)
		return XMFLOAT3{ ai_color.r, ai_color.g, ai_color.b,};
	else
		return default_value;
}

float GameMeshImport::GetNumerFactorAssimp(std::string key, unsigned int type, unsigned int slot, aiMaterial* ai_material, float defalut_value/*=0.0f*/)
{
	float value=0.0f;

	if (ai_material->Get(key.c_str(),type,slot, defalut_value)==AI_SUCCESS)
	{
		return defalut_value;
	}
	else
	{
		return defalut_value;
	}
	
}



/*Texture2D  GameMeshImport::TextureFormFile(const char* path, std::string directory)
{
	std::string filename = string(path);
	filename = directory + "/" + filename;
	std::string name = std::string(filename.begin(), filename.end());
	const char* szname = name.c_str();
	//const char* lastname = w2c(szname);
	int width = 0;
	int height = 0;
	int channelcount = 0;
	unsigned char* data = stbi_load(szname, &width, &height, &channelcount, 4);
	Texture2D* texture2d = new Texture2D;
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
	texture2d->RequestIndepentShaderReourcesForMIPs(true);
	texture2d->RequesIndenpentUnorderedAccessResoucesForMips(true);
	HRESULT hr = Renderer::GetDevice()->CreateTexture2D(&desc, InitData, &texture2d);
	assert(SUCCEEDED(hr));

	if (texture2d != nullptr)
	{
		/////
	}

	return *texture2d;
}*/

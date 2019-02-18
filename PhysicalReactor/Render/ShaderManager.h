#pragma once
#include "MISC/MiscHelper.h"
#include "Allocator/Allocator.h"
#include "Render/GraphicsResource.h"
#include "Render/GraphicsDescriptors.h"
#include "Render/Material.h"
#include <hash_map>
#include <string>

using namespace std;
using namespace PRE;



class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();
	
	hash_map<string, VertexShader*> VSMap;
	hash_map<string, PixelShader*> PSMap;
	hash_map<string, GeometryShader*> GSMap;
	hash_map<string, HullShader*> HSMap;
	hash_map<string, DomainShader*> DSMap;
	hash_map<string, ComputerShader*> CSMap;
	hash_map<string, VertexLayout*> VLMap;
	void LoadShader(const std::string& pathfilename,SHADERSTAGE type);
	void CreateShader();
	void CreateVertexLayout(const std::string& filename,GraphicBlob* Graphicblob);
	VertexShader* GetVertexShader(const std::string& filename);
	PixelShader* GetPixelShader(const std::string& filename);
	HullShader* GetHullShader(const std::string& filename);
	DomainShader* GetDomainShader(const std::string& filename);
	GeometryShader* GetGeoShader(const std::string& filename);
	ComputerShader* GetComputerShader(const std::string& filename);
	VertexLayout* GetVertexLayout(const std::string& filename);
	void GetPSO(Material* material,GraphicPSO* PSO);
};
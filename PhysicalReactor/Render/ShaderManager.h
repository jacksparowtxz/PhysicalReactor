#pragma once
#include "MISC/MiscHelper.h"
#include "Allocator/Allocator.h"
#include "Render/GraphicsResource.h"
#include "Render/GraphicsDescriptors.h"
#include "Render/Material.h"
#include <unordered_map>
#include <string>

using namespace std;
using namespace PRE;



class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();
	
	unordered_map<string, VertexShader*> VSMap;
	unordered_map<string, PixelShader*> PSMap;
	unordered_map<string, GeometryShader*> GSMap;
	unordered_map<string, HullShader*> HSMap;
	unordered_map<string, DomainShader*> DSMap;
	unordered_map<string, ComputerShader*> CSMap;
	unordered_map<string, VertexLayout*> VLMap;
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
	void GetPSO(Material* material,GraphicPSO* pso);
	void GetPSO(OBJECTTYPE objectype, GraphicPSO* pso);
};
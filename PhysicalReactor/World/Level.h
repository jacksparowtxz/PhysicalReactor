#pragma once
#include"Container/vector.h"
#include"Render/Renderable.h"
#include"Render/Sky.h"
#include"Allocator/Allocator.h"

using namespace PRE;

class Level
{
public:
	Level(Allocator* Inallocator);
	~Level();
	
	Vector<StaticMesh> StaticMeshList;
	Vector<DirectionalLight> DirectionalLightList;
	Vector<SpotLight> SpotLightList;
	Vector<PointLight> PointLightList;
	Vector<SkyLight> SkyLightList;

	Sky* sky;


	Allocator* allocator;
	void AddStaticMesh(StaticMesh* sm);
	void AddSky();
	void AddLight(LightType lighttype, XMFLOAT3 position, XMFLOAT3 rotation);
};


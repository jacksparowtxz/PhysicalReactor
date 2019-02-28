#pragma once
#include"Container/vector.h"
#include"Render/Renderable.h"
#include"Render/Sky.h"
#include"Allocator/Allocator.h"

using namespace PRE;

class Level
{
public:
	Level(Allocator* allocator);
	~Level();
	
	Vector<StaticMesh*> StaticMeshList;
	Vector<Light*> LightList;


	Sky* sky;


	Allocator* allocator;
	void AddStaticMesh(StaticMesh* sm);
	void AddSky();
	void AddLight(LightType lighttype);
};


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

	Sky* sky;


	Allocator* allocator;
	void AddStaticMesh(StaticMesh* sm);
	void AddSky();
};


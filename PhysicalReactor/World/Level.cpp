#include "stdafx.h"
#include "Level.h"

Level::Level(Allocator *allocator) :StaticMeshList(*allocator), LightList(*allocator),sky(nullptr)
{
	AddSky();

	ComputerPSO CSPSO;
	CSPSO.desc.cs == Renderer::shadermanager->GetComputerShader("equirect2cube.hlsl");

}

Level::~Level()
{
	allocatorFC::deallocateDelete(*allocator, sky);
}


void Level::AddStaticMesh(StaticMesh* sm)
{
	StaticMeshList.Push_Back(sm);
}

void Level::AddSky()
{
	allocatorFC::allocateNew<Sky>(*allocator, sky);
}

void Level::AddLight(LightType lighttype)
{
	Light* light;
	switch (lighttype)
	{
	case DIRECTIONALLIGHT:
	{
		light=allocatorFC::allocateNew<DirectionalLight>(*allocator);
	}
	break;
	case POINTLIGHT:
	{
		light = allocatorFC::allocateNew<PointLight>(*allocator);
	}
	break;
	case SPOTLIGHT:
	{
		light = allocatorFC::allocateNew<SpotLight>(*allocator);
	}
	break;
	case SKYLIGHT:
	{
		light = allocatorFC::allocateNew<SkyLight>(*allocator);
	}
	default:
		break;
	}
	LightList.Push_Back(light);
}

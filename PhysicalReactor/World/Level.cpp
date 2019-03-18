#include "stdafx.h"
#include "Level.h"

Level::Level(Allocator *allocator) :StaticMeshList(*allocator), 
                                    DirectionalLightList(*allocator), 
                                    SpotLightList(*allocator), 
                                    PointLightList(*allocator), 
                                    SkyLightList(*allocator),
                                    sky(nullptr)
{
	AddSky();
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


void Level::AddLight(LightType lighttype, XMFLOAT3 position, XMFLOAT3 rotation)
{

	switch (lighttype)
	{
	case DIRECTIONALLIGHT:
	{
		DirectionalLightList.Push_Back(allocatorFC::allocateNew<DirectionalLight>(*allocator, position));
	}
	break;
	case POINTLIGHT:
	{
		PointLightList.Push_Back(allocatorFC::allocateNew<PointLight>(*allocator, position, rotation));
	}
	break;
	case SPOTLIGHT:
	{
		SpotLightList.Push_Back(allocatorFC::allocateNew<SpotLight>(*allocator, position, rotation));
	}
	break;
	case SKYLIGHT:
	{
		SkyLightList.Push_Back(allocatorFC::allocateNew<SkyLight>(*allocator));
	}
	default:
		break;
	}
}
#include "stdafx.h"
#include "Level.h"

Level::Level() :sky(nullptr)                    
{
	AddSky();
}

Level::~Level()
{	
	SAFE_DELETE(sky);
}


void Level::AddStaticMesh(StaticMesh* sm)
{
	StaticMeshList.push_back(sm);
}

void Level::AddSky()
{
   sky=new Sky;
}


void Level::AddLight(LightType lighttype, XMFLOAT3 position, XMFLOAT3 rotation)
{

	switch (lighttype)
	{
	case DIRECTIONALLIGHT:
	{
		DirectionalLight *dl = new DirectionalLight(position);
		DirectionalLightList.emplace_back(dl);
	}
	break;
	case POINTLIGHT:
	{
		PointLight* pl = new PointLight(position,rotation);
		PointLightList.emplace_back(pl);
	}
	break;
	case SPOTLIGHT:
	{
		SpotLight* sl = new SpotLight(position, rotation);
		SpotLightList.emplace_back(sl);
	}
	break;
	case SKYLIGHT:
	{
		SkyLight* skl = new SkyLight;
		SkyLightList.emplace_back(skl);
	}
	default:
		break;
	}
}
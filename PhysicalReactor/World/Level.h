#pragma once

#include"Render/Renderable.h"
#include"Render/Sky.h"
#include<vector>

using namespace PRE;
using namespace std;
class Level
{
public:
	Level();
	~Level();
	
	vector<StaticMesh*> StaticMeshList;
	vector<DirectionalLight*> DirectionalLightList;
	vector<SpotLight*> SpotLightList;
	vector<PointLight*> PointLightList;
	vector<SkyLight*> SkyLightList;
	vector<SphereAreaLight*> SphereLightList;
	vector<TubeLight*> TubeLightList;
	vector<DiscLight*> DiscLightList;
	vector<RectangleLight*> RectangleLightList;
	Sky* sky;



	void AddStaticMesh(StaticMesh* sm);
	void AddSky();
	void AddLight(LightType lighttype, XMFLOAT3 position, XMFLOAT3 rotation);
};


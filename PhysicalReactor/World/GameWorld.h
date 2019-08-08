#pragma once

#include "World/RenderWorld.h"
#include "MISC/MeshLoad.h"
#include "Render/Renderable.h"
#include "MISC/AssetManager.h"
#include "MISC/TextureManager.h"
#include "World/Level.h"
#include <windows.h>

using namespace PRE;



class GameWorld
{
public:

	RenderWorld* renderworld;
	
	Level* level;

	
	GameWorld(HWND hwnd);
	void Update(double deltatime);
	void Render();


	void ReSize(int width, int height);
	void MoveForward(float Direction);
	void MoveRight(float Direction);
	void CameraRotation(WPARAM btnState,int x,int y);
	void MouseButtonDown(HWND windows, int x, int y);
	void MouseButtonUp();

	void BuildScene();
	void AddStaticMesh(std::string path, XMFLOAT3 position = {0.0f,0.0f,0.0f}, XMFLOAT3 rotation = { 0.0f,0.0f,0.0f }, XMFLOAT3 scale = { 1.0f,1.0f,1.0f });

	void AddLight(LightType lighttype, XMFLOAT3 position, XMFLOAT3 rotation);
	~GameWorld();
};


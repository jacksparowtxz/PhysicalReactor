#pragma once
#include"Allocator/Allocator.h"
#include"Allocator/ProxyAllocator.h"
#include"Allocator/DynamicLinearAllocator.h"
#include"Allocator/FreeListAllocator.h"
#include"Allocator/DynamicLinearAllocator.h"
#include "World/RenderWorld.h"
#include "MISC/MeshLoad.h"
#include "Render/Renderable.h"

#include <windows.h>

using namespace PRE;

class GameWorld
{
public:

	Allocator* Main_Allocator;
	ProxyAllocator* RenderAllocator;
	DynamicLinearAllocator* dynamiclinearallocator;
	void* memory;
	const char* title;
	RenderWorld* renderworld;
	
	GameMeshImport* MesnImport;
	bool bGamePause;
	bool bMinized;
	bool bMaximized;
	GameWorld(HWND hwnd);
	void Update(double deltatime);
	void Render();

	void GamePause();
	void GameResume();
	void ReSize(int width, int height);
	void MoveForward(float Direction);
	void MoveRight(float Direction);
	void CameraRotation(int x,int y);
	void GameMinize(bool min);
	void GameMaxize(bool max);
	bool GetMinize();
	bool GetMaxize();
	
	void Import(std::string path);
	~GameWorld();
};


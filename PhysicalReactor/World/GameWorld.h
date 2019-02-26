#pragma once
#include"Allocator/Allocator.h"
#include"Allocator/ProxyAllocator.h"
#include"Allocator/DynamicLinearAllocator.h"
#include"Allocator/FreeListAllocator.h"
#include"Allocator/DynamicLinearAllocator.h"
#include "World/RenderWorld.h"
#include "MISC/MeshLoad.h"
#include "Render/Renderable.h"
#include "MISC/AssetManager.h"
#include "MISC/TextureManager.h"
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
	
	
	GameWorld(HWND hwnd);
	void Update(double deltatime);
	void Render();


	void ReSize(int width, int height);
	void MoveForward(float Direction);
	void MoveRight(float Direction);
	void CameraRotation(WPARAM btnState,int x,int y);
	void MouseButtonDown(HWND windows, int x, int y);
	void MouseButtonUp();
	
	void Import(std::string path);
	~GameWorld();
};


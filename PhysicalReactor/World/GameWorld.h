#pragma once
#include"Allocator/Allocator.h"
#include"Allocator/ProxyAllocator.h"
#include"Allocator/DynamicLinearAllocator.h"
#include"Allocator/FreeListAllocator.h"
#include "World/RenderWorld.h"
#include "MISC/Timer.h"

#include <windows.h>

using namespace PRE;

class GameWorld
{
public:

	Allocator* Main_Allocator;
	ProxyAllocator* RenderAllocator;
	void* memory;
	const char* title;
	RenderWorld* renderworld;
	Timer *gametimer;

	GameWorld(HWND hwnd);
	void Update(double deltatime);
	void Render();
	void GameStart();
	void GameStop();
	void ReSize(int width, int height);
	void MoveForward(float Direction);
	void MoveRight(float Direction);
	void CameraRotation(int x,int y);
	~GameWorld();
};


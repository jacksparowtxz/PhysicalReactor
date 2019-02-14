#pragma once
#include"Allocator/Allocator.h"
#include"Allocator/ProxyAllocator.h"
#include"Allocator/DynamicLinearAllocator.h"
#include"Allocator/FreeListAllocator.h"
#include "World/RenderWorld.h"
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
	GameWorld(HWND hwnd);
	~GameWorld();
};


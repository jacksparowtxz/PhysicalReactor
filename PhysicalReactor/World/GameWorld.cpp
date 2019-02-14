#include "stdafx.h"
#include "GameWorld.h"
#include "Allocator/MemoryUtil.h"

GameWorld::GameWorld(HWND hwnd)
{
	size_t memory_size = 128 * 1024 * 1024;
	memory = malloc(memory_size);
	Main_Allocator = new (memory)FreeListAllocator(memory_size - sizeof(FreeListAllocator), add(memory, sizeof(FreeListAllocator)));
	ASSERT(Main_Allocator != nullptr);
	RenderAllocator = allocatorFC::allocateNew<ProxyAllocator>(*Main_Allocator, *Main_Allocator);
	ASSERT(RenderAllocator != nullptr);

	renderworld = allocatorFC::allocateNew<RenderWorld>(*RenderAllocator, hwnd, RenderAllocator);



}


GameWorld::~GameWorld()
{
}

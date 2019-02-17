#include "stdafx.h"
#include "GameWorld.h"
#include "Allocator/MemoryUtil.h"
#include <string>

GameWorld::GameWorld(HWND hwnd)
{
	size_t memory_size = 128 * 1024 * 1024;
	memory = malloc(memory_size);
	Main_Allocator = new (memory)FreeListAllocator(memory_size - sizeof(FreeListAllocator), add(memory, sizeof(FreeListAllocator)));
	ASSERT(Main_Allocator != nullptr);
	RenderAllocator = allocatorFC::allocateNew<ProxyAllocator>(*Main_Allocator, *Main_Allocator);
	ASSERT(RenderAllocator != nullptr);

	renderworld = allocatorFC::allocateNew<RenderWorld>(*RenderAllocator, hwnd, RenderAllocator);

	gametimer = allocatorFC::allocateNew<Timer>(*Main_Allocator);

	MesnImport = allocatorFC::allocateNew<GameMeshImport>(*Main_Allocator);

	const char* tmptitle = "PRE";
	size_t titlelength = strlen(tmptitle) + 1;
	char* tmp = allocatorFC::allocateArrayNoConstruct<char>(*Main_Allocator, titlelength);
	memcpy(tmp, tmptitle, sizeof(char)*titlelength);
	title = tmp;

	bGamePause = false;
	bMinized = false;
	bMaximized = false;
}

void GameWorld::Update(double deltatime)
{
	renderworld->Update(deltatime);
}

void GameWorld::Render()
{
	if (!bGamePause)
	{
		renderworld->RenderScene();
	}
}


void GameWorld::GameTimeStart()
{
	gametimer->Start();
}

void GameWorld::GameTimeStop()
{
	gametimer->Stop();
}

void GameWorld::GamePause()
{
	bGamePause = true;
}

void GameWorld::GameResume()
{
	bGamePause = false;
}

void GameWorld::ReSize(int width, int height)
{
	renderworld->ReSize(width,height);
}

void GameWorld::MoveForward(float Direction)
{
	renderworld->MoveForWard(Direction);
}

void GameWorld::MoveRight(float Direction)
{
	renderworld->MoveRight(Direction);
}

void GameWorld::CameraRotation(int x, int y)
{
	renderworld->CameraRotation(x, y);
}

void GameWorld::GameMinize(bool min)
{
	bMinized = min;
}

void GameWorld::GameMaxize(bool max)
{
	bMaximized = max;
}

bool GameWorld::GetMinize()
{
	return bMinized;
}

bool GameWorld::GetMaxize()
{
	return bMaximized;
}



void GameWorld::Import(std::string path)
{
	StaticMesh* ImportMesh = allocatorFC::allocateNew<StaticMesh>(*Main_Allocator);
	MesnImport->Import(path,ImportMesh);
	renderworld->AddStaticMesh(ImportMesh);
}

GameWorld::~GameWorld()
{
	if (Main_Allocator != nullptr)
	{
		if (RenderAllocator != nullptr)
		{
			allocatorFC::deallocateDelete(*Main_Allocator, RenderAllocator);
		}
		allocatorFC::deallocateArray(*Main_Allocator, (char*)title);
		free(memory);
	}

}

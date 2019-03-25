#include "stdafx.h"
#include "GameWorld.h"
#include "Allocator/MemoryUtil.h"
#include <string>

GameWorld::GameWorld(HWND hwnd)
{
	size_t memory_size = 512 * 1024 * 1024;
	memory = malloc(memory_size);
	Main_Allocator = new (memory)FreeListAllocator(memory_size - sizeof(FreeListAllocator), add(memory, sizeof(FreeListAllocator)));
	ASSERT(Main_Allocator != nullptr);
	RenderAllocator = allocatorFC::allocateNew<ProxyAllocator>(*Main_Allocator, *Main_Allocator);
	ASSERT(RenderAllocator != nullptr);
	AssetManager::MeshImport = allocatorFC::allocateNew<GameMeshImport>(*Main_Allocator);
	ASSERT(AssetManager::MeshImport != nullptr);
	dynamiclinearallocator = allocatorFC::allocateNew<DynamicLinearAllocator>(*Main_Allocator, *Main_Allocator, 512 * 1024, 32);
	ASSERT(dynamiclinearallocator != nullptr);
	renderworld = allocatorFC::allocateNew<RenderWorld>(*RenderAllocator, hwnd, RenderAllocator,dynamiclinearallocator);
	ASSERT(renderworld!= nullptr);
	TextureManager::TextureImport = allocatorFC::allocateNew<TextureLoader>(*Main_Allocator);
	

	const char* tmptitle = "PRE";
	size_t titlelength = strlen(tmptitle) + 1;
	char* tmp = allocatorFC::allocateArrayNoConstruct<char>(*Main_Allocator, titlelength);
	memcpy(tmp, tmptitle, sizeof(char)*titlelength);
	title = tmp;

	level = allocatorFC::allocateNew<Level>(*Main_Allocator, dynamiclinearallocator);
	BuildScene();
}

void GameWorld::Update(double deltatime)
{
	renderworld->Update(deltatime);
}

void GameWorld::Render()
{
	renderworld->RenderScene();
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

void GameWorld::CameraRotation(WPARAM btnState,int x, int y)
{
	renderworld->CameraRotation(btnState,x, y);
}

void GameWorld::MouseButtonDown(HWND windows, int x, int y)
{
	renderworld->SetMousePosition(windows,x, y);
}

void GameWorld::MouseButtonUp()
{
	ReleaseCapture();
}



void GameWorld::BuildScene()
{
	AddLight(LightType::DIRECTIONALLIGHT, { 1.0f,-1.0f,1.0f }, { 0.0f,0.0f,0.0f });
	AddStaticMesh("Re/SciFiHelmet.gltf");
	renderworld->UpdateScene(level);
}

void GameWorld::AddStaticMesh(std::string path)
{
	StaticMesh* ImportMesh = allocatorFC::allocateNew<StaticMesh>(*Main_Allocator);
	AssetManager::GetDevice()->Import(path, ImportMesh);
	level->AddStaticMesh(ImportMesh);
}



void GameWorld::AddLight(LightType lighttype, XMFLOAT3 position, XMFLOAT3 rotation)
{
	level->AddLight(lighttype, position, rotation);
}
GameWorld::~GameWorld()
{
	if (Main_Allocator != nullptr)
	{
		if (RenderAllocator != nullptr)
		{
			allocatorFC::deallocateDelete(*Main_Allocator,level);
			allocatorFC::deallocateDelete(*Main_Allocator, AssetManager::MeshImport);
			allocatorFC::deallocateDelete(*Main_Allocator, TextureManager::TextureImport);
			allocatorFC::deallocateDelete(*RenderAllocator, renderworld);
			allocatorFC::deallocateDelete(*Main_Allocator, RenderAllocator);
			allocatorFC::deallocateDelete(*Main_Allocator, dynamiclinearallocator);
		}
		allocatorFC::deallocateArray(*Main_Allocator, (char*)title);
		free(memory);
	}

}

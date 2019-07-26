#include "stdafx.h"
#include "GameWorld.h"
#include <string>

GameWorld::GameWorld(HWND hwnd)
{

	AssetManager::MeshImport =new GameMeshImport;
	renderworld =new RenderWorld(hwnd);
	TextureManager::TextureImport = new TextureLoader;
	level = new Level;
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
	
	AddLight(LightType::DIRECTIONALLIGHT, { 0.57735f, -0.57735f, 0.57735f }, { 1.0f,1.0f,1.0f });
	
	
	AddStaticMesh("Re/DamagedHelmet.gltf");  // DamagedHelmet
	renderworld->UpdateScene(level);
}

void GameWorld::AddStaticMesh(std::string path)
{
	StaticMesh* ImportMesh = new StaticMesh;
	AssetManager::GetDevice()->Import(path, ImportMesh);
	level->AddStaticMesh(ImportMesh);
}



void GameWorld::AddLight(LightType lighttype, XMFLOAT3 position, XMFLOAT3 rotation)
{
	level->AddLight(lighttype, position, rotation);
}
GameWorld::~GameWorld()
{
	GameMeshImport *tempGMI = AssetManager::MeshImport;
	SAFE_DELETE(tempGMI);
	AssetManager::MeshImport = nullptr;
	TextureLoader* tempTL = TextureManager::TextureImport;
	SAFE_DELETE(tempTL);
	TextureManager::TextureImport = nullptr;
	SAFE_DELETE(level);
	SAFE_DELETE(renderworld);
}

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
	
	//AddLight(LightType::DIRECTIONALLIGHT, { 0.57735f, -0.57735f, 0.57735f }, { 1.0f,1.0f,1.0f });
	//AddLight(LightType::SPOTLIGHT, { 0.0f, -3.0f, 0.0f }, { 0.0f,-90.0f,0.0f });
	
	AddStaticMesh("Re/DamagedHelmet.gltf",XMFLOAT3(0.0f,0.0f,0.0f)); 
	//AddStaticMesh("Re/TwoSidedPlane.gltf", XMFLOAT3(0.0f, -5.0f, 0.0f), XMFLOAT3(25.0f, 00.0f, 12.0f), XMFLOAT3(10.0f, 1.0f, 10.0f));
	renderworld->UpdateScene(level);
}

void GameWorld::AddStaticMesh(std::string path, XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 scale)
{
	StaticMesh* ImportMesh = new StaticMesh(position,rotation,scale);
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

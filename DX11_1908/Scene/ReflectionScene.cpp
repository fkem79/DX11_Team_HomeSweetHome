#include "Framework.h"
#include "ReflectionScene.h"

ReflectionScene::ReflectionScene()
{
	sky = new CubeSky();
	sphere = new Sphere(L"Reflection");	
	terrain = new Terrain();
	terrain->position = 
		Vector3(-terrain->GetSize().x * 0.5f, -5.0f, -terrain->GetSize().y * 0.5f);
	reflection = new Reflection(sphere);
}

ReflectionScene::~ReflectionScene()
{
	delete sky;
	delete sphere;
	delete terrain;
	delete reflection;
}

void ReflectionScene::Update()
{
	sphere->Update();
	terrain->Update();
	reflection->Update();
}

void ReflectionScene::PreRender()
{
	reflection->PreRender();

	sky->Render();
	terrain->Render();	
}

void ReflectionScene::Render()
{
	sky->Render();

	reflection->Render();
	sphere->Render();
	terrain->Render();
}

void ReflectionScene::PostRender()
{
	reflection->PostRender();
}

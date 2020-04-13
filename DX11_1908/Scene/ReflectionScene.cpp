#include "Framework.h"
#include "ReflectionScene.h"

ReflectionScene::ReflectionScene()
{
	sky = new CubeSky();
	//sphere = new Sphere(L"Reflection");	
	//sphere = new Sphere(L"Refraction");
	sphere = new Sphere();
	terrain = new Terrain();
	terrain->position = 
		Vector3(-terrain->GetSize().x * 0.5f, -5.0f, -terrain->GetSize().y * 0.5f);
	reflection = new Reflection(terrain);
	refraction = new Refraction(L"Textures/Landscape/WaveNormal.png");
}

ReflectionScene::~ReflectionScene()
{
	delete sky;
	delete sphere;
	delete terrain;
	delete reflection;
	delete refraction;
}

void ReflectionScene::Update()
{
	sphere->Update();
	terrain->Update();
	reflection->Update();
	//refraction->Update();
}

void ReflectionScene::PreRender()
{
	reflection->PreRender();
	//refraction->PreRender();

	sky->Render();
	sphere->Render();
	//terrain->Render();	
}

void ReflectionScene::Render()
{
	sky->Render();

	reflection->Render();
	//refraction->Render();
	sphere->Render();
	terrain->Render();
}

void ReflectionScene::PostRender()
{
	reflection->PostRender();
	//refraction->PostRender();
}

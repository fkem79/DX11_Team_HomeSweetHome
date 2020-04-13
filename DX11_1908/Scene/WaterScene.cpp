#include "Framework.h"
#include "WaterScene.h"

WaterScene::WaterScene()
{
	water = new Water(20.0f);
	sphere = new Sphere();
	sky = new CubeSky();
}

WaterScene::~WaterScene()
{
	delete water;
	delete sphere;
	delete sky;
}

void WaterScene::Update()
{
	water->Update();
	sphere->Update();
	
}

void WaterScene::PreRender()
{
	water->PreRenderReflection();
	sky->Render();
	sphere->Render();
	water->PreRenderRefraction();
	sky->Render();
	sphere->Render();
}

void WaterScene::Render()
{
	sky->Render();
	water->Render();
	sphere->Render();
}

void WaterScene::PostRender()
{
}

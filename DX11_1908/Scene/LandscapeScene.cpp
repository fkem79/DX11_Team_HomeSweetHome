#include "Framework.h"
#include "LandscapeScene.h"

LandscapeScene::LandscapeScene()
{
	sphere = new Sphere(1.0f, 40, 20);
	terrain = new Terrain();
}

LandscapeScene::~LandscapeScene()
{
	delete sphere;
	delete terrain;
}

void LandscapeScene::Update()
{
	//sphere->position.SetY(terrain->GetHeight(sphere->position) + sphere->GetRadius());

	sphere->Update();
	terrain->Update();
}

void LandscapeScene::PreRender()
{
	
}

void LandscapeScene::Render()
{
	sphere->Render();
	terrain->Render();
}

void LandscapeScene::PostRender()
{
	terrain->PostRender();

	Vector3 temp;
	//terrain->Picking(&temp);
	terrain->ComputePicking(&temp);

	ImGui::Text("x : %f, y : %f, z : %f", temp.GetX(), temp.GetY(), temp.GetZ());
}

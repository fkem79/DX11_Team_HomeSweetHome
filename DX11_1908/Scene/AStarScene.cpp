#include "Framework.h"
#include "AStarScene.h"

AStarScene::AStarScene()
{
	terrain = new Terrain();
	kaya = new Kaya();
	sky = new CubeSky();
	aStar = new AStar();

	aStar->Setup(terrain);
	kaya->SetTerrain(terrain);
	kaya->SetAStar(aStar);

	CAMERA->SetTarget(kaya->GetTransform());
}

AStarScene::~AStarScene()
{
	delete terrain;
	delete kaya;
	delete sky;
	delete aStar;
}

void AStarScene::Update()
{
	terrain->Update();
	kaya->Update();
	aStar->Update();
}

void AStarScene::PreRender()
{
}

void AStarScene::Render()
{
	terrain->Render();
	kaya->Render();
	aStar->Render();
}

void AStarScene::PostRender()
{
}

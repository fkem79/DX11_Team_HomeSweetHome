#include "Framework.h"
#include "AStarScene.h"

AStarScene::AStarScene()
{
	terrain = new Terrain();
	kaya = new Kaya();
	belle = new Belle();
	sky = new CubeSky();
	aStar = new AStar();
	aStar->Setup(terrain);


//	CAMERA->SetTarget(kaya->GetTransform());
	CAMERA->SetTarget(belle->GetTransform());
}

AStarScene::~AStarScene()
{
	delete terrain;
	delete kaya;
	delete sky;
	delete aStar;

	delete belle;
}

void AStarScene::Update()
{
	terrain->Update();
//	kaya->Update();
	belle->Update();
	aStar->Update();
}

void AStarScene::PreRender()
{
}

void AStarScene::Render()
{
	terrain->Render();
	belle->Render();
//	kaya->Render();
	//aStar->Render();
}

void AStarScene::PostRender()
{
}

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

	playerHaed = new PlayerHead();
	playerLHand = new PlayerLHand();

	playerLHand->GetTransform()->SetParent(&playerHaed->GetTransform()->GetWorld());
	//playerLHand->GetTransform()->position = { 0.0f, -15.0f, 0.0f };

	belle->GetTransform()->position = { 30.0f, 0.0f, 50.0f };

	int a;
	CAMERA->SetTarget(kaya->GetTransform());
//	CAMERA->SetTarget(belle->GetTransform());
	//CAMERA->SetTarget(playerHaed->GetTransform());
//	CAMERA->SetTarget(playerLHand->GetTransform());
}

AStarScene::~AStarScene()
{
	delete terrain;
	delete kaya;
	delete sky;
	delete aStar;

	delete belle;
	delete playerHaed;
	delete playerLHand;
}

void AStarScene::Update()
{
	terrain->Update();
//	kaya->Update();
	belle->Update();
	aStar->Update();
	playerHaed->Update();
	playerLHand->Update();
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

	playerHaed->Render();
	playerLHand->Render();
}

void AStarScene::PostRender()
{
}

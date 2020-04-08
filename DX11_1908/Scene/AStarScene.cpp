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

	playerHaed->GetTransform()->position = { 10.0f, 0.0f, 15.0f };
	playerHaed->GetTransform()->rotation = { 0, 180.0f, 0 };
	//playerLHand->GetTransform()->SetParent(&playerHaed->GetTransform()->GetWorld());
	playerLHand->GetTransform()->position = { 5.0f, 10.0f, 5.0f };

	belle->GetTransform()->position = { 30.0f, 0.0f, 50.0f };

//	CAMERA->SetTarget(belle->GetTransform());
	CAMERA->SetTarget(playerHaed->GetTransform());
	//CAMERA->SetTarget(playerLHand->GetTransform());
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
	belle->Update();
	aStar->Update();

	playerHaed->Update();
	playerLHand->Update();

	//playerLHand->GetTransform()->UpdateWorld();
}

void AStarScene::PreRender()
{
}

void AStarScene::Render()
{
	terrain->Render();
	belle->Render();

	playerHaed->Render();
//	playerLHand->Render();
}

void AStarScene::PostRender()
{
}

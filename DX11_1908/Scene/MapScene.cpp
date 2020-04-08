#include "Framework.h"
#include "MapScene.h"

MapScene::MapScene()
{
	
	belle = new Belle();
	playerHaed = new PlayerHead();
	playerLHand = new PlayerLHand();

	//doorWall = new DoorWall();

	playerHaed->GetTransform()->position = { 10.0f, 0.0f, 15.0f };
	playerHaed->GetTransform()->rotation = { 0, 180.0f, 0 };
	//playerLHand->GetTransform()->SetParent(&playerHaed->GetTransform()->GetWorld());
	//playerLHand->GetTransform()->position = { 5.0f, 10.0f, 5.0f };

	belle->GetTransform()->position = { 30.0f, 0.0f, 50.0f };
	//doorWall->GetTransform()->position = { 40.0f, 0.0f, 50.0f };

	//	CAMERA->SetTarget(belle->GetTransform());
	CAMERA->SetTarget(playerHaed->GetTransform());
	//CAMERA->SetTarget(playerLHand->GetTransform());
}

MapScene::~MapScene()
{
	delete belle;
	delete playerHaed;
	delete playerLHand;
}

void MapScene::Update()
{
	belle->Update();
	//doorWall->Update();

	playerHaed->Update();
	playerLHand->Update();


	//playerLHand->GetTransform()->UpdateWorld();
}

void MapScene::PreRender()
{
}

void MapScene::Render()
{
	belle->Render();

	playerHaed->Render();
	//	playerLHand->Render();
	//doorWall->Render();
}

void MapScene::PostRender()
{
}

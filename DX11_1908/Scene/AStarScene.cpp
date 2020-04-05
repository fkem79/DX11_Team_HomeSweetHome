#include "Framework.h"
#include "AStarScene.h"

AStarScene::AStarScene()
{
	terrain = new Terrain();
	kaya = new Kaya();
<<<<<<< HEAD
	belle = new Belle();
	sky = new CubeSky();
	aStar = new AStar();
	aStar->Setup(terrain);


//	CAMERA->SetTarget(kaya->GetTransform());
	CAMERA->SetTarget(belle->GetTransform());
=======
	sky = new CubeSky();
	aStar = new AStar();

	aStar->Setup(terrain);
	kaya->SetTerrain(terrain);
	kaya->SetAStar(aStar);

	CAMERA->SetTarget(kaya->GetTransform());
>>>>>>> sub/master
}

AStarScene::~AStarScene()
{
	delete terrain;
	delete kaya;
	delete sky;
	delete aStar;
<<<<<<< HEAD

	delete belle;
=======
>>>>>>> sub/master
}

void AStarScene::Update()
{
	terrain->Update();
<<<<<<< HEAD
//	kaya->Update();
	belle->Update();
=======
	kaya->Update();
>>>>>>> sub/master
	aStar->Update();
}

void AStarScene::PreRender()
{
}

void AStarScene::Render()
{
	terrain->Render();
<<<<<<< HEAD
	belle->Render();
//	kaya->Render();
	//aStar->Render();
=======
	kaya->Render();
	aStar->Render();
>>>>>>> sub/master
}

void AStarScene::PostRender()
{
}

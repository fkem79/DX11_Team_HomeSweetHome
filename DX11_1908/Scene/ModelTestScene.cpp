#include "Framework.h"
#include "ModelTestScene.h"

ModelTestScene::ModelTestScene()
{
	model = new ModelSingle("tile_big01");
}

ModelTestScene::~ModelTestScene()
{
	delete model;
}

void ModelTestScene::Update()
{
	model->Update();
}

void ModelTestScene::PreRender()
{
}

void ModelTestScene::Render()
{
	model->Render();
}

void ModelTestScene::PostRender()
{
}

#include "Framework.h"
#include "ScatteringScene.h"

ScatteringScene::ScatteringScene()
{
	sky = new Scattering();
}

ScatteringScene::~ScatteringScene()
{
	delete sky;
}

void ScatteringScene::Update()
{
	sky->Update();
}

void ScatteringScene::PreRender()
{
	sky->PreRender();
}

void ScatteringScene::Render()
{
	sky->Render();
}

void ScatteringScene::PostRender()
{
	sky->PostRender();
}

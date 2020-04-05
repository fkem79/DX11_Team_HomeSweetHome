#include "Framework.h"
#include "ShadowScene.h"

ShadowScene::ShadowScene()
{
	terrain = new Terrain();
	kaya = new Kaya();

	shadow = new Shadow();		
}

ShadowScene::~ShadowScene()
{
	delete terrain;
	delete kaya;

	delete shadow;
}

void ShadowScene::Update()
{
	terrain->Update();
	kaya->Update();

	kaya->GetTransform()->position.SetY(terrain->GetHeight(kaya->GetTransform()->position));
}

void ShadowScene::PreRender()
{
	shadow->PreRender();

	terrain->SetShader(Shader::Add(L"Terrain", "VS", "PS_Depth"));
	kaya->GetModel()->SetShader(
		Shader::Add(L"ModelAnimationInstancing", "VS", "PS_Depth")
	);

	//terrain->Render();	
	kaya->Render();
}

void ShadowScene::Render()
{
	terrain->SetShader(Shader::Add(L"Terrain", "VS", "PS_Shadow"));
	kaya->GetModel()->SetShader(Shader::Add(L"ModelAnimationInstancing"));

	shadow->SetShadowBuffer();

	terrain->Render();
	kaya->Render();
}

void ShadowScene::PostRender()
{	
	shadow->PostRender();
}

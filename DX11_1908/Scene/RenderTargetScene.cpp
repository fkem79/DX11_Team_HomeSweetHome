#include "Framework.h"
#include "RenderTargetScene.h"
#include "Objects/UI/RadialBlur.h"

RenderTargetScene::RenderTargetScene()
{
	terrain = new Terrain();
	sky = new CubeSky();

	renderTarget = new RenderTarget();
	mrt[0] = new RenderTarget();
	mrt[1] = new RenderTarget();

	depthStencil = new DepthStencil();

	render2D = new Render2D(L"MRT");
	render2D->scale = Vector3(WIN_WIDTH, WIN_HEIGHT, 1.0f);
	render2D->position = Vector3(WIN_WIDTH * 0.5f, WIN_HEIGHT * 0.5f, 0.0f);
	render2D->SetSRV(renderTarget->GetSRV());

	targetTexture = new Render2D();
	targetTexture->scale = Vector3(WIN_WIDTH * 0.5f, WIN_HEIGHT * 0.5f, 1.0f);
	targetTexture->position = Vector3(WIN_WIDTH * 0.25f, WIN_HEIGHT * 0.25f, 0.0f);
	targetTexture->SetSRV(mrt[0]->GetSRV());

	radialBlur = new RadialBlur();
	radialBlur ->scale = Vector3(WIN_WIDTH * 0.5f, WIN_HEIGHT * 0.5f, 1.0f);
	radialBlur ->position = Vector3(WIN_WIDTH * 0.75f, WIN_HEIGHT * 0.25f, 0.0f);
	radialBlur ->SetSRV(mrt[1]->GetSRV());
}

RenderTargetScene::~RenderTargetScene()
{
	delete terrain;
	delete sky;

	delete renderTarget;
	delete targetTexture;
	delete radialBlur;
	delete render2D;

	delete mrt[0];
	delete mrt[1];
}

void RenderTargetScene::Update()
{
	render2D->Update();
	terrain->Update();
	
	targetTexture->Update();
	radialBlur->Update();
}

void RenderTargetScene::PreRender()
{
	renderTarget->Set(depthStencil);

	sky->Render();
	terrain->Render();	

	RenderTarget::Sets(mrt, 2, depthStencil);	
	render2D->Render();
}

void RenderTargetScene::Render()
{
	terrain->Render();
}

void RenderTargetScene::PostRender()
{
	targetTexture->Render();
	radialBlur->Render();
}

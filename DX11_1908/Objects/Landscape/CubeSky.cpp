#include "Framework.h"

CubeSky::CubeSky()
{
	sphere = new Sphere(L"CubeSky");
	sphere->GetFillMode()->FrontCounterClockwise(true);

	cubeMap = Texture::Add(L"Textures/Landscape/Space.dds");

	depthMode[0] = new DepthStencilState();
	depthMode[1] = new DepthStencilState();
	depthMode[1]->DepthEnable(false);
}

CubeSky::~CubeSky()
{
	delete sphere;

	delete depthMode[0];
	delete depthMode[1];
}

void CubeSky::Render()
{
	cubeMap->Set(10);

	depthMode[1]->SetState();

	sphere->Render();

	depthMode[0]->SetState();
}

#include "Framework.h"

Refraction::Refraction(wstring normalFile)
{
	renderTarget = new RenderTarget();
	depthStencil = new DepthStencil();
	
	timeBuffer = new FloatBuffer();

	targetTexture = new Render2D();
	targetTexture->SetSRV(renderTarget->GetSRV());
	targetTexture->scale = Vector3(300, 300, 300);
	targetTexture->position = Vector3(150, 150, 0);

	normalMap = Texture::Add(normalFile);
}

Refraction::~Refraction()
{
	delete renderTarget;
	delete depthStencil;	
	delete timeBuffer;
	delete targetTexture;
}

void Refraction::Update()
{
	targetTexture->Update();
	timeBuffer->data.indices[0] += DELTA;
}

void Refraction::PreRender()
{
	renderTarget->Set(depthStencil);
}

void Refraction::Render()
{
	timeBuffer->SetPSBuffer(10);
	normalMap->Set(2);
	DC->PSSetShaderResources(11, 1, &renderTarget->GetSRV());
}

void Refraction::PostRender()
{
	targetTexture->Render();
}

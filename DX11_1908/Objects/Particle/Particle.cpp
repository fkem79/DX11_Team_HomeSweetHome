#include "Framework.h"
#include "Particle.h"

Particle::Particle(wstring file, UINT particleCount)
	: particleCount(particleCount), isPlay(false)
{
	texture = Texture::Add(file);

	blendState[0] = new BlendState();
	blendState[1] = new BlendState();
	blendState[1]->Additive();

	depthState[0] = new DepthStencilState();
	depthState[1] = new DepthStencilState();
	depthState[1]->DepthWriteMask(D3D11_DEPTH_WRITE_MASK_ZERO);
}

Particle::~Particle()
{
	delete vertexBuffer;

	delete blendState[0];
	delete blendState[1];

	delete depthState[0];
	delete depthState[1];
}

void Particle::Update()
{
}

void Particle::Render()
{
	blendState[1]->SetState();	
	depthState[1]->SetState();

	worldBuffer->SetVSBuffer(1);
	vertexBuffer->Set();
	IASetPT(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	VP->SetGSBuffer(0);

	texture->Set(10);
	shader->Set();

	DC->Draw(particleCount, 0);

	DC->GSSetShader(nullptr, nullptr, 0);

	blendState[0]->SetState();
	depthState[0]->SetState();
}

void Particle::Create()
{
}

void Particle::Play(Vector3 position)
{
	isPlay = true;
	this->position = position;
}

void Particle::Stop()
{
	isPlay = false;
}

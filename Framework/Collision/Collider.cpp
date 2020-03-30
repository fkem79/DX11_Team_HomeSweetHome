#include "Framework.h"

Collider::Collider()
{
	shader = Shader::Add(L"Collider");
	colorBuffer = new ColorBuffer();
	colorBuffer->data.color = Float4(0, 1, 0, 1);
}

Collider::~Collider()
{
	delete colorBuffer;
	delete vertexBuffer;
	delete indexBuffer;
}

void Collider::Render()
{
	UpdateWorld();

	vertexBuffer->Set();
	indexBuffer->Set();
	IASetPT(D3D_PRIMITIVE_TOPOLOGY_LINELIST);

	colorBuffer->SetPSBuffer(10);
	worldBuffer->SetVSBuffer(1);

	shader->Set();

	DC->DrawIndexed(indices.size(), 0, 0);
}

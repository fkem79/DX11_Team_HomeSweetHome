#include "Framework.h"

Render2D::Render2D(wstring shaderFile)
{
	shader = Shader::Add(shaderFile);

	view = XMMatrixIdentity();
	orthographic = XMMatrixOrthographicOffCenterLH(0, Device::Get()->GetWidth(), 0, Device::Get()->GetHeight(), -1, 1);

	depthState = new DepthStencilState();
	depthState->DepthEnable(false);

	VertexUV vertices[6];
	vertices[0].position = { -0.5f, -0.5f, 0.0f };
	vertices[1].position = { -0.5f, 0.5f, 0.0f };
	vertices[2].position = { 0.5f, -0.5f, 0.0f };

	vertices[3].position = { 0.5f, -0.5f, 0.0f };
	vertices[4].position = { -0.5f, 0.5f, 0.0f };
	vertices[5].position = { 0.5f, 0.5f, 0.0f };

	vertices[0].uv = { 0.0f, 1.0f };
	vertices[1].uv = { 0.0f, 0.0f };
	vertices[2].uv = { 1.0f, 1.0f };

	vertices[3].uv = { 1.0f, 1.0f };
	vertices[4].uv = { 0.0f, 0.0f };
	vertices[5].uv = { 1.0f, 0.0f };

	vertexBuffer = new VertexBuffer(vertices, sizeof(VertexUV), 6);
}

Render2D::~Render2D()
{
	delete depthState;
	delete vertexBuffer;
}

void Render2D::Update()
{
	UpdateWorld();
}

void Render2D::Render()
{
	VP->SetView(view);
	VP->SetProjection(orthographic);
	VP->SetVSBuffer(0);

	vertexBuffer->Set();
	IASetPT();

	DC->PSSetShaderResources(10, 1, &srv);

	worldBuffer->SetVSBuffer(1);

	shader->Set();

	ID3D11DepthStencilState* oldState;
	UINT stencilRef;
	DC->OMGetDepthStencilState(&oldState, &stencilRef);

	depthState->SetState();
	DC->Draw(6, 0);
	DC->OMSetDepthStencilState(oldState, stencilRef);
}

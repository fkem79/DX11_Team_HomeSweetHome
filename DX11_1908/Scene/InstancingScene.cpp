#include "Framework.h"
#include "InstancingScene.h"

InstancingScene::InstancingScene()
{
	material = new Material(L"Instancing");
	material->SetDiffuseMap(L"Textures/test.jpg");

	for (UINT i = 0; i < COUNT; i++)
	{
		Float3 pos;
		pos.x = Random(-50.0f, 50.0f);
		pos.y = Random(-50.0f, 50.0f);
		pos.z = 0.0f;

		worlds[i] = XMMatrixTranslation(pos.x, pos.y, pos.z);
		worlds[i] = XMMatrixTranspose(worlds[i]);
	}

	worldBuffer = new WorldBuffer();

	instanceBuffer = new VertexBuffer(worlds, sizeof(Matrix), COUNT);

	CreateData();

	//vertexBuffers[0] = vertexBuffer->GetBuffer();
	//vertexBuffers[1] = instanceBuffer->GetBuffer();
}

InstancingScene::~InstancingScene()
{
	delete material;
	delete vertexBuffer;
	delete worldBuffer;
}

void InstancingScene::Update()
{
}

void InstancingScene::PreRender()
{
}

void InstancingScene::Render()
{
	vertexBuffer->Set();
	instanceBuffer->Set(1);
	indexBuffer->Set();

	//UINT stride[2] = { sizeof(VertexUV), sizeof(Matrix) };
	//UINT offset[2] = { 0, 0 };
	//DC->IASetVertexBuffers(0, 2, vertexBuffers, stride, offset);

	IASetPT();

	material->Set();

	//DC->DrawInstanced(6, COUNT, 0, 0);
	DC->DrawIndexedInstanced(indices.size(), COUNT, 0, 0, 0);

	/*
	for (UINT i = 0; i < COUNT; i++)
	{
		worldBuffer->SetWorld(worlds[i]);
		worldBuffer->SetVSBuffer(1);

		DC->Draw(6, 0);
	}*/

}

void InstancingScene::PostRender()
{
}

void InstancingScene::CreateData()
{
	vertices.resize(4);

	vertices[0].position = Float3(-1, -1, 0);
	vertices[1].position = Float3(-1, 1, 0);
	vertices[2].position = Float3(1, 1, 0);	
	vertices[3].position = Float3(1, -1, 0);

	vertices[0].uv = Float2(0, 1);
	vertices[1].uv = Float2(0, 0);
	vertices[2].uv = Float2(1, 0);
	vertices[3].uv = Float2(1, 1);

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexUV), vertices.size());

	indices = { 0, 1, 2, 0, 2, 3 };

	indexBuffer = new IndexBuffer(indices.data(), indices.size());
}


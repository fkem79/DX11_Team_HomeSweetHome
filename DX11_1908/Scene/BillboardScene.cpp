#include "Framework.h"
#include "BillboardScene.h"
#include "Objects/Billboard.h"

BillboardScene::BillboardScene()
	: count(1000)
{
	terrain = new Terrain();

	/*
	for (UINT i = 0; i < 1000; i++)
	{
		Billboard* tree = new Billboard(L"Textures/Landscape/Tree.png");

		Vector3 pos;
		pos.SetX(Random(0.0f, 255.0f));
		pos.SetZ(Random(0.0f, 255.0f));

		Vector3 scale;
		scale.SetX(Random(3.0f, 10.0f));
		scale.SetY(Random(3.0f, 10.0f));
		scale.SetZ(1.0f);

		pos.SetY(scale.GetY() * 0.5f + terrain->GetHeight(pos));

		tree->position = pos;
		tree->scale = scale;

		trees.push_back(tree);
	}*/

	VertexSize* vertices = new VertexSize[count];

	for (UINT i = 0; i < count; i++)
	{
		Vector3 pos;
		pos.SetX(Random(0.0f, 255.0f));
		pos.SetZ(Random(0.0f, 255.0f));

		Vector3 scale;
		scale.SetX(Random(3.0f, 10.0f));
		scale.SetY(Random(3.0f, 10.0f));
		scale.SetZ(1.0f);

		pos.SetY(scale.GetY() * 0.5f + terrain->GetHeight(pos));

		vertices[i].position = pos;
		vertices[i].size.x = scale.GetX();
		vertices[i].size.y = scale.GetY();
	}

	vertexBuffer = new VertexBuffer(vertices, sizeof(VertexSize), count);

	delete[] vertices;

	shader = Shader::Add(L"GeometryBillboard");
	shader->CreateGeometryShader();

	texture = Texture::Add(L"Textures/Landscape/Tree.png");

	blendMode[0] = new BlendState();
	blendMode[1] = new BlendState();
}

BillboardScene::~BillboardScene()
{
	delete terrain;

	//for (Billboard* tree : trees)
		//delete tree;

	delete vertexBuffer;

	delete blendMode[0];
	delete blendMode[1];
}

void BillboardScene::Update()
{
	if (KEYDOWN(VK_F1))
		blendMode[1]->Alpha(true);
	if (KEYDOWN(VK_F2))
		blendMode[1]->AlphaToCoverage(true);
	if (KEYDOWN(VK_F3))
		blendMode[1]->IndependentBlend(true);
	if (KEYDOWN(VK_F4))
		blendMode[1]->Additive();

	terrain->Update();

	//for (Billboard* tree : trees)
		//tree->Update();
}

void BillboardScene::PreRender()
{
}

void BillboardScene::Render()
{
	terrain->Render();

	blendMode[1]->SetState();
	//for (Billboard* tree : trees)
		//tree->Render();

	vertexBuffer->Set();
	IASetPT(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	VP->SetGSBuffer(0);

	texture->Set(10);
	shader->Set();

	DC->Draw(count, 0);

	DC->GSSetShader(nullptr, nullptr, 0);

	blendMode[0]->SetState();
}

void BillboardScene::PostRender()
{
}

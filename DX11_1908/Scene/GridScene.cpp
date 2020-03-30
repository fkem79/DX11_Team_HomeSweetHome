#include "Framework.h"
#include "GridScene.h"

GridScene::GridScene()
	: width(10), height(10)
{
	shader = Shader::Add(L"Grid");
	worldBuffer = new WorldBuffer();	

	D3D11_RASTERIZER_DESC desc = {};
	desc.AntialiasedLineEnable = false;
	desc.CullMode = D3D11_CULL_BACK;
	desc.FillMode = D3D11_FILL_WIREFRAME;

	DEVICE->CreateRasterizerState(&desc, &state);

	texture = Texture::Add(L"Textures/test.jpg");
	heightMap = Texture::Add(L"Textures/HeightMaps/ColorMap256.png");

	width = heightMap->GetWidth() - 1;
	height = heightMap->GetHeight() - 1;

	CreateData();
}

GridScene::~GridScene()
{
	delete worldBuffer;
	delete vertexBuffer;
	delete indexBuffer;

	state->Release();
}

void GridScene::Update()
{
}

void GridScene::PreRender()
{
}

void GridScene::Render()
{
	//DC->RSSetState(state);

	vertexBuffer->Set();
	indexBuffer->Set();
	IASetPT();

	worldBuffer->SetVSBuffer(1);

	texture->Set(0);

	shader->Set();

	DC->DrawIndexed(indices.size(), 0, 0);
}

void GridScene::PostRender()
{
}

void GridScene::CreateData()
{
	vector<Float4> colors = heightMap->ReadPixels();

	//Vertices
	for (UINT z = 0; z <= height; z++)
	{
		for (UINT x = 0; x <= width; x++)
		{
			VertexType vertex;
			vertex.position = Float3(x, 0, z);
			vertex.uv = Float2(x / (float)width, 1.0f - z / (float)height);

			UINT index = (height - z) * (width + 1) + x;
			vertex.position.y = colors[index].y * 50.0f;

			vertices.push_back(vertex);
		}
	}

	//Indices
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			indices.push_back((width + 1) * z + x); //0
			indices.push_back((width + 1) * (z + 1) + x);//1
			indices.push_back((width + 1) * (z + 1) + x + 1);//2

			indices.push_back((width + 1) * z + x); //0
			indices.push_back((width + 1) * (z + 1) + x + 1);//2
			indices.push_back((width + 1) * z + x + 1);//3
		}
	}

	CreateNormal();

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexType), vertices.size());
	indexBuffer = new IndexBuffer(indices.data(), indices.size());
}

void GridScene::CreateNormal()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		Vector3 v0 = vertices[index0].position;
		Vector3 v1 = vertices[index1].position;
		Vector3 v2 = vertices[index2].position;

		Vector3 A = v1 - v0;
		Vector3 B = v2 - v0;

		Vector3 normal = Vector3::Cross(A, B).Normal();

		Vector3 n0 = vertices[index0].normal;
		Vector3 n1 = vertices[index1].normal;
		Vector3 n2 = vertices[index2].normal;

		vertices[index0].normal = (n0 + normal).Normal();
		vertices[index1].normal = (n1 + normal).Normal();
		vertices[index2].normal = (n2 + normal).Normal();
	}
}

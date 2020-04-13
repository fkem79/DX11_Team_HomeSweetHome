#include "Framework.h"
#include "Water.h"

Water::Water(float radius)
	: radius(radius), width(WIN_WIDTH), height(WIN_HEIGHT)
{
	material = new Material(L"Water");

	vertexCount = 4;
	indexCount = 6;

	VertexUV vertices[4];
	vertices[0].position = Vector3(-radius, 0.0f, -radius);
	vertices[1].position = Vector3(-radius, 0.0f, +radius);
	vertices[2].position = Vector3(+radius, 0.0f, -radius);
	vertices[3].position = Vector3(+radius, 0.0f, +radius);

	vertices[0].uv = Float2(0, 1);
	vertices[1].uv = Float2(0, 0);
	vertices[2].uv = Float2(1, 1);
	vertices[3].uv = Float2(1, 0);

	UINT indices[6] = { 0, 1, 2, 2, 1, 3 };

	vertexBuffer = new VertexBuffer(vertices, sizeof(VertexUV), vertexCount);
	indexBuffer = new IndexBuffer(indices, indexCount);

	buffer = new WaterBuffer();

	reflection = new Reflection(this);
	refraction = new Refraction(L"Textures/Landscape/Wave.dds");
}

Water::~Water()
{
	delete material;

	delete vertexBuffer;
	delete indexBuffer;

	delete buffer;

	delete reflection;
	delete refraction;
}

void Water::Update()
{
	buffer->data.waveTranslation += buffer->data.waveSpeed * DELTA;
	if (buffer->data.waveTranslation > 1.0f)
		buffer->data.waveTranslation -= 1.0f;

	reflection->Update();
	refraction->Update();

	UpdateWorld();
}

void Water::PreRenderReflection()
{
	reflection->PreRender();
}

void Water::PreRenderRefraction()
{
	refraction->PreRender();
}

void Water::Render()
{
	worldBuffer->SetVSBuffer(1);

	reflection->Render();
	refraction->Render();

	buffer->SetPSBuffer(10);

	vertexBuffer->Set();
	indexBuffer->Set();
	IASetPT();

	material->Set();

	DC->DrawIndexed(indexCount, 0, 0);
}

#include "Framework.h"
#include "Billboard.h"

Billboard::Billboard(wstring diffuseFile)
{
	material = new Material(L"Billboard");
	material->SetDiffuseMap(diffuseFile);

	CreateData();
}

Billboard::~Billboard()
{
	delete material;
	delete vertexBuffer;
	delete indexBuffer;
}

void Billboard::Update()
{
	//Vector3 rot = CAMERA->rotation;
	//rotation.SetY(rot.GetY());

	Vector3 camPos = CAMERA->position;
	Vector3 temp = position - camPos;

	rotation.SetY(atan2(temp.GetX(), temp.GetZ()));

	UpdateWorld();
}

void Billboard::Render()
{
	vertexBuffer->Set();
	indexBuffer->Set();
	IASetPT();

	worldBuffer->SetVSBuffer(1);
	material->Set();

	DC->DrawIndexed(6, 0, 0);
}

void Billboard::CreateData()
{
	VertexUV vertices[4];
	vertices[0].position = Float3(-0.5f, 0.5f, 0.0f);
	vertices[1].position = Float3(0.5f, 0.5f, 0.0f);
	vertices[2].position = Float3(-0.5f, -0.5f, 0.0f);
	vertices[3].position = Float3(0.5f, -0.5f, 0.0f);

	vertices[0].uv = Float2(0, 0);
	vertices[1].uv = Float2(1, 0);
	vertices[2].uv = Float2(0, 1);
	vertices[3].uv = Float2(1, 1);

	vertexBuffer = new VertexBuffer(vertices, sizeof(VertexUV), 4);

	UINT indices[] = {
		0, 1, 2,
		1, 3, 2
	};

	indexBuffer = new IndexBuffer(indices, 6);
}

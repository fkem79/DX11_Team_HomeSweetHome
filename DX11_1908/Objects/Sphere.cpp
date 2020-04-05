#include "Framework.h"
#include "Sphere.h"

Sphere::Sphere(float radius, UINT sliceCount, UINT stackCount)
	: radius(radius), sliceCount(sliceCount), stackCount(stackCount)
{
	material = new Material(L"Terrain");
	material->SetDiffuseMap(L"Textures/Landscape/Fieldstone_DM.tga");
	material->SetSpecularMap(L"Textures/Landscape/Fieldstone_SM.tga");
	material->SetNormalMap(L"Textures/Landscape/Fieldstone_NM.tga");

	intBuffer = new IntBuffer();
	CreateData();
	CreateTangent();

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexType), vertices.size());
	indexBuffer = new IndexBuffer(indices.data(), indices.size());	

	fillMode[0] = new RasterizerState();
	fillMode[1] = new RasterizerState();
	//fillMode[1]->FillMode(D3D11_FILL_WIREFRAME);
}

Sphere::Sphere(wstring shaderFile, float radius, UINT sliceCount, UINT stackCount)
	: radius(radius), sliceCount(sliceCount), stackCount(stackCount)
{
	material = new Material(shaderFile);	

	intBuffer = new IntBuffer();
	CreateData();
	CreateTangent();

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexType), vertices.size());
	indexBuffer = new IndexBuffer(indices.data(), indices.size());

	fillMode[0] = new RasterizerState();
	fillMode[1] = new RasterizerState();	
}

Sphere::~Sphere()
{
	delete vertexBuffer;
	delete indexBuffer;
	delete intBuffer;

	delete fillMode[0];
	delete fillMode[1];
}

void Sphere::Update()
{
	UpdateWorld();
}

void Sphere::Render()
{
	vertexBuffer->Set();
	indexBuffer->Set();
	IASetPT();

	worldBuffer->SetVSBuffer(1);
	intBuffer->SetPSBuffer(10);
	
	material->Set();

	fillMode[1]->SetState();

	DC->DrawIndexed(indices.size(), 0, 0);

	fillMode[0]->SetState();

	//ImGui::SliderInt("SpecularMap", &intBuffer->data.indices[0], 0, 1);
	//ImGui::SliderInt("NormalMap", &intBuffer->data.indices[1], 0, 1);
}

void Sphere::CreateData()
{
	float phiStep = XM_PI / stackCount;
	float thetaStep = XM_2PI / sliceCount;

	for (UINT i = 0; i <= stackCount; i++)
	{
		float phi = i * phiStep;

		for (UINT j = 0; j <= sliceCount; j++)
		{
			float theta = j * thetaStep;

			VertexType vertex;

			vertex.normal.x = sin(phi) * cos(theta);
			vertex.normal.y = cos(phi);
			vertex.normal.z = sin(phi) * sin(theta);

			vertex.position = Vector3(vertex.normal) * radius;

			vertex.uv.x = (float)j / sliceCount;
			vertex.uv.y = (float)i / stackCount;

			vertices.push_back(vertex);
		}
	}

	for (UINT i = 0; i < stackCount; i++)
	{
		for (UINT j = 0; j < sliceCount; j++)
		{
			indices.push_back((sliceCount + 1) * i + j); //0
			indices.push_back((sliceCount + 1) * i + j + 1); //1
			indices.push_back((sliceCount + 1) * (i + 1) + j); //2

			indices.push_back((sliceCount + 1) * (i + 1) + j); //2
			indices.push_back((sliceCount + 1) * i + j + 1); //1
			indices.push_back((sliceCount + 1) * (i + 1) + (j + 1)); //3
		}
	}
}

void Sphere::CreateTangent()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		VertexType vertex0 = vertices[index0];
		VertexType vertex1 = vertices[index1];
		VertexType vertex2 = vertices[index2];

		Vector3 p0 = vertex0.position;
		Vector3 p1 = vertex1.position;
		Vector3 p2 = vertex2.position;

		Float2 uv0 = vertex0.uv;
		Float2 uv1 = vertex1.uv;
		Float2 uv2 = vertex2.uv;

		Vector3 e0 = p1 - p0;
		Vector3 e1 = p2 - p0;

		float u0 = uv1.x - uv0.x;
		float u1 = uv2.x - uv0.x;
		float v0 = uv1.y - uv0.y;
		float v1 = uv2.y - uv0.y;

		float d = 1.0f / (u0 * v1 - v0 * u1);

		Vector3 tangent;
		tangent = (v1 * e0 - v0 * e1) * d;

		vertices[index0].tangent = tangent + vertices[index0].tangent;
		vertices[index1].tangent = tangent + vertices[index1].tangent;
		vertices[index2].tangent = tangent + vertices[index2].tangent;
	}

	for (VertexType& vertex : vertices)
	{
		Vector3 t = vertex.tangent;
		Vector3 n = vertex.normal;

		Vector3 temp = (t - n * Vector3::Dot(n, t)).Normal();

		vertex.tangent = temp;
	}
}

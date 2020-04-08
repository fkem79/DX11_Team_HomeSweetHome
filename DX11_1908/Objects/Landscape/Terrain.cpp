#include "Framework.h"
#include "Terrain.h"

Terrain::Terrain()
{
	material = new Material(L"Terrain");
	material->SetDiffuseMap(L"Textures/Landscape/Wall.png");
	material->SetSpecularMap(L"Textures/Landscape/Wall_specular.png");
	material->SetNormalMap(L"Textures/Landscape/Wall_normal.png");

	intBuffer = new IntBuffer();		
	
	heightMap = Texture::Add(L"Textures/HeightMaps/HeightMap.png");

	width = heightMap->GetWidth() - 1;
	height = heightMap->GetHeight() - 1;

	CreateData();
	CreateNormal();
	CreateTangent();

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexType), vertices.size());
	indexBuffer = new IndexBuffer(indices.data(), indices.size());

	cShader = new ComputeShader(L"Intersection");

	size = 1 * (indices.size() / 3);

	structuredBuffer = new StructuredBuffer(input, sizeof(InputDesc), size, sizeof(OutputDesc), size);

	rayBuffer = new RayBuffer();

	output = new OutputDesc[size];
}

Terrain::~Terrain()
{
	delete material;
	delete intBuffer;
	delete vertexBuffer;
	delete indexBuffer;	

	delete cShader;
	delete rayBuffer;
	delete structuredBuffer;

	delete[] input;
	delete[] output;
}

void Terrain::Update()
{
	UpdateWorld();
}

void Terrain::Render()
{
	vertexBuffer->Set();
	indexBuffer->Set();
	IASetPT();

	intBuffer->SetPSBuffer(10);
	worldBuffer->SetVSBuffer(1);

	material->Set();

	DC->DrawIndexed(indices.size(), 0, 0);
}

void Terrain::PostRender()
{
	ImGui::Text("Terrain Option");
	ImGui::SliderInt("IsSpecularMap", &intBuffer->data.indices[0], 0, 1);
	ImGui::SliderInt("IsNormalMap", &intBuffer->data.indices[1], 0, 1);
	ImGui::SliderFloat3("MaterialDiffuse", (float*)&material->GetBuffer()->data.diffuse, 0, 1);
	ImGui::SliderFloat3("MaterialSpecular", (float*)&material->GetBuffer()->data.specular, 0, 1);
	ImGui::SliderFloat3("MaterialAmbient", (float*)&material->GetBuffer()->data.ambient, 0, 1);
}

bool Terrain::Picking(OUT Vector3* position)
{
	Ray ray = CAMERA->GetRay();

	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			UINT index[4];
			index[0] = (width + 1) * z + x;
			index[1] = (width + 1) * z + x + 1;
			index[2] = (width + 1) * (z + 1) + x;
			index[3] = (width + 1) * (z + 1) + x + 1;

			Vector3 p[4];
			for (UINT i = 0; i < 4; i++)
				p[i] = vertices[index[i]].position;

			float distance;
			if (IntersectTri(ray.position, ray.direction,
				p[0], p[1], p[2], &distance))
			{
				*position = ray.position + distance * ray.direction;
				return true;
			}

			if (IntersectTri(ray.position, ray.direction,
				p[3], p[1], p[2], &distance))
			{
				*position = ray.position + distance * ray.direction;
				return true;
			}
		}
	}

	return false;
}

bool Terrain::ComputePicking(OUT Vector3* position)
{
	Ray ray = CAMERA->GetRay();
	rayBuffer->data.position = ray.position;
	rayBuffer->data.direction = ray.direction;
	rayBuffer->data.padding = size;
	cShader->Set();

	rayBuffer->SetCSBuffer(0);

	DC->CSSetShaderResources(0, 1, &structuredBuffer->SRV());
	DC->CSSetUnorderedAccessViews(0, 1, &structuredBuffer->UAV(), nullptr);

	UINT x = ceil((float)size / 1024.0f);

	DC->Dispatch(x, 1, 1);
	
	structuredBuffer->Copy(output, sizeof(OutputDesc) * size);

	for (UINT i = 0; i < size; i++)
	{
		OutputDesc temp = output[i];
		if (temp.picked)
		{
			*position = ray.position + ray.direction * temp.distance;
			return true;
		}
	}

	return false;
}

float Terrain::GetHeight(Vector3 position)
{
	UINT x = (UINT)position.GetX();
	UINT z = (UINT)position.GetZ();

	if (x < 0 || x > width) return 0.0f;
	if (z < 0 || z > width) return 0.0f;

	UINT index[4];
	index[0] = (width + 1) * z + x;
	index[1] = (width + 1) * (z + 1) + x;
	index[2] = (width + 1) * z + x + 1;
	index[3] = (width + 1) * (z + 1) + x + 1;	

	Vector3 p[4];
	for (int i = 0; i < 4; i++)
		p[i] = vertices[index[i]].position;

	float u = (position.GetX() - p[0].GetX());
	float v = (position.GetZ() - p[0].GetZ());

	Vector3 result;
	if (u + v <= 1.0f)
	{
		result = p[0] + (p[2] - p[0]) * u + (p[1] - p[0]) * v;
	}
	else
	{
		u = 1.0f - u;
		v = 1.0f - v;

		result = p[3] + (p[1] - p[3]) * u + (p[2] - p[3]) * v;
	}

	return result.GetY();
}

void Terrain::CreateData()
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

	input = new InputDesc[indices.size() / 3];	
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		input[i].v0 = vertices[index0].position;
		input[i].v1 = vertices[index1].position;
		input[i].v2 = vertices[index2].position;

		input[i].index = i;
	}
}

void Terrain::CreateNormal()
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

void Terrain::CreateTangent()
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

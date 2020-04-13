#include "Framework.h"
#include "TerrainData.h"

TerrainData::TerrainData(wstring heightFile)
{
	heightMap = Texture::Add(heightFile);

	CreateData();
	CreateNormal();
	AlignVertexData();
}

TerrainData::~TerrainData()
{
}

void TerrainData::CreateData()
{
	vector<Float4> colors = heightMap->ReadPixels();

	width = heightMap->GetWidth() - 1;
	height = heightMap->GetHeight() - 1;
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

			prevData.push_back(vertex);
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
}

void TerrainData::CreateNormal()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		Vector3 v0 = prevData[index0].position;
		Vector3 v1 = prevData[index1].position;
		Vector3 v2 = prevData[index2].position;

		Vector3 A = v1 - v0;
		Vector3 B = v2 - v0;

		Vector3 normal = Vector3::Cross(A, B).Normal();

		Vector3 n0 = prevData[index0].normal;
		Vector3 n1 = prevData[index1].normal;
		Vector3 n2 = prevData[index2].normal;

		prevData[index0].normal = (n0 + normal).Normal();
		prevData[index1].normal = (n1 + normal).Normal();
		prevData[index2].normal = (n2 + normal).Normal();
	}
}

void TerrainData::AlignVertexData()
{
	vertices.resize(indices.size());

	UINT index = 0;
	for (UINT z = 0; z < height; z++)
	{
		for (UINT x = 0; x < width; x++)
		{
			UINT index1 = (width + 1) * z + x;
			UINT index2 = (width + 1) * z + x + 1;
			UINT index3 = (width + 1) * (z + 1) + x;
			UINT index4 = (width + 1) * (z + 1) + x + 1;

			vertices[index++] = prevData[index3];
			vertices[index++] = prevData[index4];
			vertices[index++] = prevData[index1];

			vertices[index++] = prevData[index1];
			vertices[index++] = prevData[index4];
			vertices[index++] = prevData[index2];
		}
	}
}

#include "Framework.h"

TerrainEditor::TerrainEditor(UINT width, UINT height)
	: width(width), height(height), isRaise(true), adjustValue(20.0f),
	isPainting(true), paintValue(0.5f), selectTextureNum(0)
{
	material = new Material(L"Splatting");
	material->SetDiffuseMap(L"Textures/Landscape/Dirt2.png");	

	secondMap = Texture::Add(L"Textures/Landscape/Box.png");
	thirdMap = Texture::Add(L"Textures/test.jpg");

	brushBuffer = new BrushBuffer();

	CreateData();
	CreateNormal();	

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(VertexType), vertices.size());
	indexBuffer = new IndexBuffer(indices.data(), indices.size());
}

TerrainEditor::~TerrainEditor()
{
	delete material;
	delete brushBuffer;
	delete vertexBuffer;
	delete indexBuffer;
}

void TerrainEditor::Update()
{
	if (KEYPRESS(VK_LBUTTON))
	{
		if (isPainting)
		{
			if (isRaise)
				PaintBrush(brushBuffer->data.location, paintValue);
			else
				PaintBrush(brushBuffer->data.location, -paintValue);
		}
		else
		{
			if (isRaise)
				AdjustY(brushBuffer->data.location, adjustValue);
			else
				AdjustY(brushBuffer->data.location, -adjustValue);
		}		
	}

	if (KEYUP(VK_LBUTTON))
	{
		CreateNormal();
		vertexBuffer->Update(vertices.data(), vertices.size());
	}

	UpdateWorld();
}

void TerrainEditor::Render()
{
	vertexBuffer->Set();
	indexBuffer->Set();
	IASetPT();

	brushBuffer->SetVSBuffer(10);
	worldBuffer->SetVSBuffer(1);

	secondMap->Set(10);
	thirdMap->Set(11);

	material->Set();

	DC->DrawIndexed(indices.size(), 0, 0);
}

void TerrainEditor::PostRender()
{	
	Vector3 temp;
	Picking(&temp);
	brushBuffer->data.location = temp;

	ImGui::Text("x : %f, y : %f, z : %f", temp.GetX(), temp.GetY(), temp.GetZ());
	ImGui::Checkbox("IsPainting", &isPainting);
	ImGui::Checkbox("IsRaise", &isRaise);	
	ImGui::SliderInt("SelectTextureNum", &selectTextureNum, 0, 1);

	if (ImGui::Button("Save"))
		Save();
	if (ImGui::Button("Load"))
		Load();
}

bool TerrainEditor::Picking(OUT Vector3* position)
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

void TerrainEditor::AdjustY(Vector3 position, float value)
{
	switch (brushBuffer->data.type)
	{
	case 1:
	{
		for (VertexType& vertex : vertices)
		{
			Vector3 p1 = Vector3(vertex.position.x, 0, vertex.position.z);
			Vector3 p2 = Vector3(position.GetX(), 0, position.GetZ());

			float dist = Distance(p1, p2);				

			float temp = value * max(0, cos(XM_PIDIV2 * dist / brushBuffer->data.range));

			if (dist <= brushBuffer->data.range)
			{
				vertex.position.y += temp * DELTA;
			}
		}
	}
		break;
	}

	vertexBuffer->Update(vertices.data(), vertices.size());
}

void TerrainEditor::PaintBrush(Vector3 position, float value)
{
	switch (brushBuffer->data.type)
	{
	case 1:
	{
		for (VertexType& vertex : vertices)
		{
			Vector3 p1 = Vector3(vertex.position.x, 0, vertex.position.z);
			Vector3 p2 = Vector3(position.GetX(), 0, position.GetZ());

			float dist = Distance(p1, p2);

			float temp = value * max(0, cos(XM_PIDIV2 * dist / brushBuffer->data.range));

			if (dist <= brushBuffer->data.range)
			{
				vertex.alpha[selectTextureNum] += value * DELTA;

				vertex.alpha[selectTextureNum] = Saturate(vertex.alpha[selectTextureNum]);
			}
		}
	}
	break;
	}

	vertexBuffer->Update(vertices.data(), vertices.size());
}

void TerrainEditor::CreateData()
{
	//Vertices
	for (UINT z = 0; z <= height; z++)
	{
		for (UINT x = 0; x <= width; x++)
		{
			VertexType vertex;
			vertex.position = Float3(x, 0, z);
			vertex.uv = Float2(x / (float)width, 1.0f - z / (float)height);

			UINT index = (height - z) * (width + 1) + x;
			vertex.position.y = 0.0f;

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
}

void TerrainEditor::CreateNormal()
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

void TerrainEditor::Save()
{	
	heights.clear();

	for (VertexType vertex : vertices)
		heights.push_back(vertex.position.y);

	BinaryWriter* writer = new BinaryWriter(L"TextData/Height.map");

	writer->UInt(heights.size());
	writer->Byte(heights.data(), sizeof(float) * heights.size());

	delete writer;
}

void TerrainEditor::Load()
{
	BinaryReader* reader = new BinaryReader(L"TextData/Height.map");

	UINT size = reader->UInt();

	heights.resize(size);
	void* data = heights.data();

	reader->Byte(&data, sizeof(float) * size);

	for (UINT i = 0; i < size; i++)
		vertices[i].position.y = heights[i];

	delete reader;

	CreateNormal();
	vertexBuffer->Update(vertices.data(), vertices.size());
}

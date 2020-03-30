#pragma once

class Terrain : public Transform
{
private:
	class RayBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			Float3 position;
			float padding;

			Float3 direction;
			float padding2;
		}data;

		RayBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.position = Float3(3.0f, 5.0f, 1.0f);
			data.direction = Float3(0, -1, 0);
		}
	};

	struct InputDesc
	{
		UINT index;
		Float3 v0, v1, v2;
	};

	struct OutputDesc
	{
		UINT picked;
		float u;
		float v;
		float distance;
	};

	typedef VertexUVNormalTangent VertexType;

	Material* material;

	IntBuffer* intBuffer;

	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	vector<VertexType> vertices;
	vector<UINT> indices;

	UINT width, height;	

	Texture* heightMap;

	ComputeShader* cShader;
	RayBuffer* rayBuffer;
	StructuredBuffer* structuredBuffer;
	InputDesc* input;
	OutputDesc* output;

	UINT size;
public:
	Terrain();
	~Terrain();

	void Update();
	void Render();
	void PostRender();
	
	bool Picking(OUT Vector3* position);
	bool ComputePicking(OUT Vector3* position);

	float GetHeight(Vector3 position);

	void SetShader(Shader* shader) { material->SetShader(shader); }

	Float2 GetSize() { return Float2(width, height); }
private:
	void CreateData();
	void CreateNormal();
	void CreateTangent();
};

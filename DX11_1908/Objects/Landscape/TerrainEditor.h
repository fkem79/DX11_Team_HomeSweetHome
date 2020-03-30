#pragma once

class TerrainEditor : public Transform
{
public:
	class BrushBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			int type;
			Float3 location;

			float range;
			Float3 color;
		}data;

		BrushBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.type = 1;
			data.location = Float3(0, 0, 0);

			data.range = 10.0f;
			data.color = Float3(0, 1, 0);
		}
	};

	struct VertexAlpha
	{
		Float3 position;
		Float2 uv;
		Float3 normal;
		float alpha[4];

		VertexAlpha()
			: position(0, 0, 0), uv(0, 0), normal(0, 0, 0), alpha{0, 0, 0, 0}
		{

		}
	};

private:
	typedef VertexAlpha VertexType;

	Material* material;

	BrushBuffer* brushBuffer;

	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	vector<VertexType> vertices;
	vector<UINT> indices;

	UINT width, height;

	Texture* secondMap;
	Texture* thirdMap;

	bool isRaise;
	float adjustValue;

	bool isPainting;
	float paintValue;

	int selectTextureNum;

	vector<float> heights;
public:
	TerrainEditor(UINT width, UINT height);
	~TerrainEditor();

	void Update();
	void Render();
	void PostRender();

	bool Picking(OUT Vector3* position);
	void AdjustY(Vector3 position, float value);
	void PaintBrush(Vector3 position, float value);

private:
	void CreateData();
	void CreateNormal();

	void Save();
	void Load();
};
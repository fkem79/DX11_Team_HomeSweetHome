#pragma once

class Water : public Transform
{
private:
	class WaterBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			Float4 refractionColor;

			float waveTranslation;
			float waveScale;
			float shininess;
			float alpha;

			float waveSpeed;
			float padding[3];
		}data;

		WaterBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.refractionColor = Float4(0.2f, 0.3f, 1.0f, 1.0f);

			data.waveTranslation = 0.0f;
			data.waveScale = 0.05f;
			
			data.shininess = 30.0f;
			data.alpha = 0.5f;
			data.waveSpeed = 0.06f;
		}
	};

	WaterBuffer* buffer;
	float radius;
	UINT width, height;

	Material* material;

	Reflection* reflection;
	Refraction* refraction;

	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	UINT vertexCount, indexCount;
public:
	Water(float radius);
	~Water();

	void Update();

	void PreRenderReflection();
	void PreRenderRefraction();

	void Render();
};
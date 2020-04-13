#pragma once

class Render2D;

class Scattering
{
private:
	class TargetBuffer : public ConstBuffer
	{
	public:
		struct Data
		{
			Float3 waveLength;
			int sampleCount;

			Float3 invWaveLength;
			float padding;

			Float3 waveLengthMie;
			float padding2;
		}data;

		TargetBuffer() : ConstBuffer(&data, sizeof(Data))
		{
			data.waveLength = Float3(0.65f, 0.57f, 0.475f);

			data.invWaveLength.x = 1.0f / pow(data.waveLength.x, 4.0f);
			data.invWaveLength.y = 1.0f / pow(data.waveLength.y, 4.0f);
			data.invWaveLength.z = 1.0f / pow(data.waveLength.z, 4.0f);

			data.waveLengthMie.x = pow(data.waveLength.x, 0.84f);
			data.waveLengthMie.y = pow(data.waveLength.y, 0.84f);
			data.waveLengthMie.z = pow(data.waveLength.z, 0.84f);

			data.sampleCount = 20;
		}
	};

	TargetBuffer* targetBuffer;
	FloatBuffer* scatterBuffer;

	float theta, phi;
	float prevTheta, prevPhi;
	float radius;

	UINT slices, stacks;

	UINT vertexCount, indexCount;
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	VertexUV* quadVertices;
	VertexBuffer* quadBuffer;

	RenderTarget* targets[2];	

	DepthStencil* depthStencil;

	Shader* scatterShader;
	Shader* targetShader;

	Render2D* rayleigh2D;
	Render2D* mie2D;

	WorldBuffer* worldBuffer;
	DepthStencilState* depthMode[2];

	Texture* starField;

public:
	Scattering();
	~Scattering();

	void Update();
	void PreRender();
	void Render();
	void PostRender();

	void CreateSphere();
	void CreateQuad();
};
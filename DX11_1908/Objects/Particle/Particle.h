#pragma once

class Particle : public Transform
{
protected:
	Shader* shader;

	VertexBuffer* vertexBuffer;

	Texture* texture;

	UINT particleCount;
	bool isPlay;

	BlendState* blendState[2];
	DepthStencilState* depthState[2];
public:
	Particle(wstring file, UINT particleCount);
	virtual ~Particle();

	virtual void Update();
	virtual void Render();

	virtual void Create();

	virtual void Play(Vector3 position);
	virtual void Stop();

	bool IsPlay() { return isPlay; }
};
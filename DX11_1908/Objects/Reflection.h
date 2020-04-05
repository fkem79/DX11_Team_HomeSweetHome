#pragma once

class Reflection
{
private:
	Transform* transform;

	FixityCam* camera;

	WorldBuffer* reflectionBuffer;

	RenderTarget* renderTarget;
	DepthStencil* depthStencil;

	Render2D* targetTexture;
public:
	Reflection(Transform* transform);
	~Reflection();

	void Update();

	void PreRender();
	void Render();
	void PostRender();
};
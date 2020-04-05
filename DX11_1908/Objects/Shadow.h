#pragma once

class Shadow
{
private:
	UINT width, height;
	float radius;

	RenderTarget* depthTarget;
	DepthStencil* depthStencil;

	ViewProjectionBuffer* vpBuffer;
	ShadowBuffer* shadowBuffer;

	Render2D* depthRender;

	SamplerState* depthSampler;
public:
	Shadow();
	~Shadow();

	void PreRender();
	void PostRender();

	void SetShadowBuffer();

	void SetVP();
};
#pragma once

class Refraction
{
private:	
	FloatBuffer* timeBuffer;

	RenderTarget* renderTarget;
	DepthStencil* depthStencil;

	Texture* normalMap;

	Render2D* targetTexture;
public:
	Refraction(wstring normalFile);
	~Refraction();

	void Update();

	void PreRender();
	void Render();
	void PostRender();
};
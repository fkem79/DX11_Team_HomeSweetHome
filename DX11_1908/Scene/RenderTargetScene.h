#pragma once

class RenderTargetScene : public Scene
{
private:
	RenderTarget* renderTarget;
	RenderTarget* mrt[2];
	DepthStencil* depthStencil;

	Render2D* targetTexture;

	class RadialBlur* radialBlur;
	Render2D* render2D;

	Terrain* terrain;
	CubeSky* sky;

public:
	RenderTargetScene();
	~RenderTargetScene();
		
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};
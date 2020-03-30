#pragma once

class RenderTarget
{
private:	
	ID3D11ShaderResourceView* srv;
	ID3D11Texture2D* rtvTexture;
	ID3D11RenderTargetView* rtv;	

public:
	RenderTarget(UINT width = WIN_WIDTH, UINT height = WIN_HEIGHT);
	~RenderTarget();

	void Set(DepthStencil* depthStencil);
	static void Sets(RenderTarget** targets, UINT count, DepthStencil* depthStencil);

	ID3D11ShaderResourceView*& GetSRV() { return srv; }
	ID3D11RenderTargetView*& GetRTV() { return rtv; }	
};
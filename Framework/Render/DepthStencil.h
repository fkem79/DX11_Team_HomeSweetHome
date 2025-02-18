#pragma once

class DepthStencil
{
private:
	ID3D11Texture2D* dsvTexture;
	ID3D11DepthStencilView* dsv;
	ID3D11ShaderResourceView* srv;

public:
	DepthStencil(UINT width = Device::Get()->GetWidth(), UINT height = Device::Get()->GetHeight());
	~DepthStencil();

	ID3D11DepthStencilView*& GetDSV() { return dsv; }
	ID3D11ShaderResourceView*& GetSRV() { return srv; }
};
#pragma once

class RasterizerState
{
private:
	D3D11_RASTERIZER_DESC desc;
	ComPtr<ID3D11RasterizerState> state;

public:
	RasterizerState();

	void SetState();

	void FillMode(D3D11_FILL_MODE value);
	void FrontCounterClockwise(bool value);

	void Changed();
};
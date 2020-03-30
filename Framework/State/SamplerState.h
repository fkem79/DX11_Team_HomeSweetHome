#pragma once

class SamplerState
{
private:
	ID3D11SamplerState* state;
	D3D11_SAMPLER_DESC desc;

public:
	SamplerState();
	~SamplerState();

	void Set(UINT slot);

	void ComparisonFunc(D3D11_COMPARISON_FUNC value);
	void Filter(D3D11_FILTER value);
	void Address(D3D11_TEXTURE_ADDRESS_MODE value);

	void Changed();
};
#pragma once

class CsResource
{
protected:
	ID3D11Resource* input;
	ID3D11ShaderResourceView* srv;

	ID3D11Resource* output;
	ID3D11UnorderedAccessView* uav;

	ID3D11Resource* result;

public:
	CsResource();
	virtual ~CsResource();

	virtual void CreateInput() = 0;
	virtual void CreateSRV() = 0;

	virtual void CreateOutput() = 0;
	virtual void CreateUAV() = 0;

	virtual void CreateResult() {}

	void CreateBuffer();

	ID3D11ShaderResourceView*& SRV() { return srv; }
	ID3D11UnorderedAccessView*& UAV() { return uav; }
};
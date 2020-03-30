#include "Framework.h"

DepthStencilState::DepthStencilState()
	: state(nullptr), desc{}
{
	desc.DepthEnable = true;
	desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	desc.DepthFunc = D3D11_COMPARISON_LESS;

	desc.StencilEnable = true;
	desc.StencilReadMask = 0xff;
	desc.StencilWriteMask = 0xff;

	desc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	desc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	desc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	desc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	Changed();
}

DepthStencilState::~DepthStencilState()
{
	state->Release();
}

void DepthStencilState::SetState(UINT stencilRef)
{
	DC->OMSetDepthStencilState(state, stencilRef);
}

void DepthStencilState::DepthEnable(bool val)
{
	desc.DepthEnable = val;
	Changed();
}

void DepthStencilState::DepthWriteMask(D3D11_DEPTH_WRITE_MASK val)
{
	desc.DepthWriteMask = val;
	Changed();
}

void DepthStencilState::Changed()
{
	if (state != nullptr)
		state->Release();

	V(DEVICE->CreateDepthStencilState(&desc, &state));
}

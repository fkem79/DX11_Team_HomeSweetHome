#include "Framework.h"
#include "BlendState.h"

BlendState::BlendState()
	: state(nullptr), desc{}
{
	desc.AlphaToCoverageEnable = false;
	desc.IndependentBlendEnable = false;

	desc.RenderTarget[0].BlendEnable = false;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	Changed();
}

BlendState::~BlendState()
{
	state->Release();
}

void BlendState::SetState()
{
	float blendFactor[] = { 0, 0, 0, 0 };
	DC->OMSetBlendState(state, blendFactor, 0xffffffff);
}

void BlendState::Alpha(bool value)
{
	desc.RenderTarget[0].BlendEnable = value;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	Changed();
}

void BlendState::AlphaToCoverage(bool value)
{
	desc.AlphaToCoverageEnable = value;

	Changed();
}

void BlendState::IndependentBlend(bool value)
{
	desc.IndependentBlendEnable = value;

	Changed();
}

void BlendState::Additive()
{
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	Changed();
}

void BlendState::Changed()
{
	if (state != nullptr)
		state->Release();

	V(DEVICE->CreateBlendState(&desc, &state));
}

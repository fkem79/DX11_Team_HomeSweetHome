#include "Framework.h"
#include "RasterizerState.h"

RasterizerState::RasterizerState()
	: state(nullptr), desc{}
{	
	desc.AntialiasedLineEnable = false;
	desc.CullMode = D3D11_CULL_BACK;
	desc.FillMode = D3D11_FILL_SOLID;

	Changed();
}

void RasterizerState::SetState()
{
	DC->RSSetState(state.Get());
}

void RasterizerState::FillMode(D3D11_FILL_MODE value)
{
	desc.FillMode = value;

	Changed();
}

void RasterizerState::FrontCounterClockwise(bool value)
{
	desc.FrontCounterClockwise = value;

	Changed();
}

void RasterizerState::Changed()
{
	V(DEVICE->CreateRasterizerState(&desc, state.ReleaseAndGetAddressOf()));
}

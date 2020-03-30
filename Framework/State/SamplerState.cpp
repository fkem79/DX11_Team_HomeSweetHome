#include "Framework.h"

SamplerState::SamplerState()
    : state(nullptr), desc {}
{    
    desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    desc.MinLOD = 0;
    desc.MaxLOD = D3D11_FLOAT32_MAX;

    Changed();
}

SamplerState::~SamplerState()
{
    state->Release();
}

void SamplerState::Set(UINT slot)
{
    DC->PSSetSamplers(slot, 1, &state);
}

void SamplerState::ComparisonFunc(D3D11_COMPARISON_FUNC value)
{
    desc.ComparisonFunc = value;
    Changed();
}

void SamplerState::Filter(D3D11_FILTER value)
{
    desc.Filter = value;
    Changed();
}

void SamplerState::Address(D3D11_TEXTURE_ADDRESS_MODE value)
{
    desc.AddressU = value;
    desc.AddressV = value;
    desc.AddressW = value;
    Changed();
}

void SamplerState::Changed()
{
    if (state != nullptr)
        state->Release();

    V(DEVICE->CreateSamplerState(&desc, &state));
}

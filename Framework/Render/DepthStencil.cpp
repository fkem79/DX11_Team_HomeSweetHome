#include "Framework.h"
#include "DepthStencil.h"

DepthStencil::DepthStencil(UINT width, UINT height)
{
    D3D11_TEXTURE2D_DESC desc = {};
    desc.Width = width;
    desc.Height = height;
    desc.MipLevels = 1;
    desc.ArraySize = 1;
    desc.Format = DXGI_FORMAT_R32_TYPELESS;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage = D3D11_USAGE_DEFAULT;
    desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;

    V(DEVICE->CreateTexture2D(&desc, nullptr, &dsvTexture));

    CD3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = DXGI_FORMAT_D32_FLOAT;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;

    V(DEVICE->CreateDepthStencilView(dsvTexture, &descDSV, &dsv));
    
    D3D11_SHADER_RESOURCE_VIEW_DESC descSRV = {};
    descSRV.Format = DXGI_FORMAT_R32_FLOAT;
    descSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    descSRV.Texture2D.MipLevels = 1;

    V(DEVICE->CreateShaderResourceView(dsvTexture, &descSRV, &srv));    
}

DepthStencil::~DepthStencil()
{
    dsv->Release();
    srv->Release();

    dsvTexture->Release();
}

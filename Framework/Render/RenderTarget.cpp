#include "Framework.h"

RenderTarget::RenderTarget(UINT width, UINT height)
{    
     D3D11_TEXTURE2D_DESC desc = {};
     desc.Width = width;
     desc.Height = height;
     desc.MipLevels = 1;
     desc.ArraySize = 1;
     desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
     desc.SampleDesc.Count = 1;
     desc.SampleDesc.Quality = 0;
     desc.Usage = D3D11_USAGE_DEFAULT;
     desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

     V(DEVICE->CreateTexture2D(&desc, nullptr, &rtvTexture));

     D3D11_RENDER_TARGET_VIEW_DESC descRTV = {};
     descRTV.Format = desc.Format;
     descRTV.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

     V(DEVICE->CreateRenderTargetView(rtvTexture, &descRTV, &rtv));

     D3D11_SHADER_RESOURCE_VIEW_DESC descSRV = {};
     descSRV.Format = desc.Format;
     descSRV.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
     descSRV.Texture2D.MipLevels = 1;

     V(DEVICE->CreateShaderResourceView(rtvTexture, &descSRV, &srv));
    
}

RenderTarget::~RenderTarget()
{
    rtvTexture->Release();    

    srv->Release();
    rtv->Release();    
}

void RenderTarget::Set(DepthStencil* depthStencil)
{
    DC->OMSetRenderTargets(1, &rtv, depthStencil->GetDSV());
    Device::Get()->Clear(Float4(1, 1, 1, 1), rtv, depthStencil->GetDSV());
}

void RenderTarget::Sets(RenderTarget** targets, UINT count, DepthStencil* depthStencil)
{
    vector<ID3D11RenderTargetView*> rtvs;

    float color[4] = { 1, 1, 1, 1 };
    for (UINT i = 0; i < count; i++)
    {
        rtvs.push_back(targets[i]->GetRTV());
        DC->ClearRenderTargetView(targets[i]->GetRTV(), color);
    }

    DC->ClearDepthStencilView(depthStencil->GetDSV(), 
        D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
    DC->OMSetRenderTargets(rtvs.size(), rtvs.data(), depthStencil->GetDSV());
}

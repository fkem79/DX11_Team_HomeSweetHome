#include "Framework.h"
#include "Device.h"

Device* Device::instance = nullptr;

Device::Device(HWND hWnd)
    : hWnd(hWnd), isVsync(true)
{
	CreateDeviceAndSwapChain();
	CreateBackBuffer();
}

Device::~Device()
{
	swapChain->Release();
	device->Release();
	context->Release();

	renderTargetView->Release();
	depthStencil->Release();
	depthStencilView->Release();
}

void Device::CreateDeviceAndSwapChain()
{
    UINT width = WIN_WIDTH;
    UINT height = WIN_HEIGHT;

    IDXGIFactory1* factory;
    CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&factory);

    IDXGIAdapter1* adapter = nullptr;
    IDXGIAdapter1* selectAdapter = nullptr;

    UINT maxSize = 0;

    for (int i = 0; factory->EnumAdapters1(i, &adapter) != DXGI_ERROR_NOT_FOUND; i++)
    {
        DXGI_ADAPTER_DESC1 desc;
        adapter->GetDesc1(&desc);

        if (desc.DedicatedVideoMemory > maxSize)
        {
            maxSize = desc.DedicatedVideoMemory;
            if(selectAdapter != nullptr)
                selectAdapter->Release();
            selectAdapter = adapter;
        }
        else
        {
            adapter->Release();
        }
    }    

    DXGI_SWAP_CHAIN_DESC sd = {};
    sd.BufferCount = 1;//백버퍼의 갯수
    sd.BufferDesc.Width = width;
    sd.BufferDesc.Height = height;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    //RefreshRate : 디스플레이 모드 갱신율(주사율 : Numerator / Denominator)
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = true;

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        selectAdapter,
        D3D_DRIVER_TYPE_UNKNOWN,
        0,
        D3D11_CREATE_DEVICE_DEBUG,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &sd,
        &swapChain,
        &device,
        nullptr,
        &context
    );
    V(hr);
    selectAdapter->Release();
    factory->Release();
}

void Device::CreateBackBuffer()
{
    ID3D11Texture2D* backBuffer;

    swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
    V(device->CreateRenderTargetView(backBuffer, nullptr, &renderTargetView));
    
    backBuffer->Release();

    D3D11_TEXTURE2D_DESC descDepth = {};
    descDepth.Width = WIN_WIDTH;
    descDepth.Height = WIN_HEIGHT;
    descDepth.MipLevels = 1;
    descDepth.ArraySize = 1;
    descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    descDepth.SampleDesc.Count = 1;
    descDepth.SampleDesc.Quality = 0;
    descDepth.Usage = D3D11_USAGE_DEFAULT;
    descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

    V(device->CreateTexture2D(&descDepth, nullptr, &depthStencil));    

    CD3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
    descDSV.Format = descDepth.Format;
    descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
        
    V(device->CreateDepthStencilView(depthStencil, &descDSV, &depthStencilView));

    SetRenderTarget();
}

void Device::SetRenderTarget(ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv)
{
    if (rtv == nullptr)
        rtv = renderTargetView;
    if (dsv == nullptr)
        dsv = depthStencilView;

    context->OMSetRenderTargets(1, &rtv, dsv);
}

void Device::Clear(Float4 color, ID3D11RenderTargetView* rtv, ID3D11DepthStencilView* dsv)
{
    if (rtv == nullptr)
        rtv = renderTargetView;
    if (dsv == nullptr)
        dsv = depthStencilView;

    context->ClearRenderTargetView(rtv, (float*)&color);
    context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);
}

void Device::Present()
{
    swapChain->Present(isVsync ? 1 : 0, 0);
}

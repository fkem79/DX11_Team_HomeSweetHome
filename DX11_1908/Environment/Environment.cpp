#include "Framework.h"
#include "Environment.h"

Environment* Environment::instance = nullptr;

Environment::Environment()
{
    viewProjection = new ViewProjectionBuffer();
    light = new LightBuffer();

	CreateViewport();
	CreatePerspective();	
    CreateSamplerState();

    //mainCamera = new FreeCam();
    //mainCamera->position = Vector3(10, 10, -45);   
    mainCamera = new FPV();
    mainCamera->position = Vector3(10.0f, 17.0f, 10.0f);
    //mainCamera = new FollowCam();
    //mainCamera = new FirstPersonView();
}

Environment::~Environment()
{
	delete viewProjection;
    delete mainCamera;
    delete light;

    linearSamp->Release();
    pointSamp->Release();
}

void Environment::CreateViewport()
{
    D3D11_VIEWPORT vp;
    vp.Width = Device::Get()->GetWidth();
    vp.Height = Device::Get()->GetHeight();
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    vp.TopLeftX = 0;
    vp.TopLeftY = 0;
    DC->RSSetViewports(1, &vp);
}

void Environment::CreatePerspective()
{
    projection = XMMatrixPerspectiveFovLH(XM_PIDIV4,
        Device::Get()->GetWidth() / (float)Device::Get()->GetHeight(), 0.1f, 1000.0f);

    viewProjection->SetProjection(projection);
}

void Environment::CreateSamplerState()
{
    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    DEVICE->CreateSamplerState(&sampDesc, &linearSamp);

    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;

    DEVICE->CreateSamplerState(&sampDesc, &pointSamp);

    DC->PSSetSamplers(0, 1, &linearSamp);
    DC->PSSetSamplers(1, 1, &pointSamp);
}

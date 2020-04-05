#include "Framework.h"
#include "TutorialScene.h"

TutorialScene::TutorialScene()   
    : state(nullptr)
{
	shader = Shader::Add(L"Diffuse");

    VertexUVNormal vertices[8];
    vertices[0].position = { -1, -1, -1 };
    vertices[1].position = { -1, 1, -1 };
    vertices[2].position = { 1, 1, -1 };
    vertices[3].position = { 1, -1, -1 };

    vertices[4].position = { -1, -1, 1 };
    vertices[5].position = { -1, 1, 1 };
    vertices[6].position = { 1, 1, 1 };
    vertices[7].position = { 1, -1, 1 };

    vertices[0].uv = { 0, 1 };
    vertices[1].uv = { 0, 0 };
    vertices[2].uv = { 1, 0 };
    vertices[3].uv = { 1, 1 };

    vertices[4].uv = { 0, 0 };
    vertices[5].uv = { 0, 1 };
    vertices[6].uv = { 1, 1 };
    vertices[7].uv = { 1, 0 };

    UINT indices[] =
    {
        //¾Õ¸é
        0, 1, 2,
        0, 2, 3,

        //À­¸é
        1, 5, 6,
        1, 6, 2,

        //¿À¸¥ÂÊ
        3, 2, 6,
        3, 6, 7,

        //¿ÞÂÊ
        0, 5, 1,
        0, 4, 5,

        //¹Ø¸é
        0, 7, 4,
        0, 3, 7,

        //µÞ¸é
        4, 6, 5,
        4, 7, 6
    };

    for (UINT i = 0; i < 12; i++)
    {
        UINT index0 = indices[i * 3 + 0];
        UINT index1 = indices[i * 3 + 1];
        UINT index2 = indices[i * 3 + 2];

        Vector3 v0 = vertices[index0].position;
        Vector3 v1 = vertices[index1].position;
        Vector3 v2 = vertices[index2].position;

        Vector3 A = v1 - v0;
        Vector3 B = v2 - v0;

        Vector3 normal = Vector3::Cross(A, B).Normal();
        
        Vector3 n0 = vertices[index0].normal;
        Vector3 n1 = vertices[index1].normal;
        Vector3 n2 = vertices[index2].normal;
               
        vertices[index0].normal = (n0 + normal).Normal();
        vertices[index1].normal = (n1 + normal).Normal();
        vertices[index2].normal = (n2 + normal).Normal();        
    }    

    vertexBuffer = new VertexBuffer(vertices, sizeof(VertexUVNormal), 8);
    indexBuffer = new IndexBuffer(indices, 36);

    worldBuffer = new WorldBuffer();
    colorBuffer = new ColorBuffer();
    lightBuffer = new LightBuffer();
        
    ScratchImage image;
    V(LoadFromWICFile(L"Textures/test.jpg", WIC_FLAGS_NONE, nullptr, image));

    V(CreateShaderResourceView(DEVICE, image.GetImages(), image.GetImageCount(),
        image.GetMetadata(), &srv));

    D3D11_SAMPLER_DESC sampDesc = {};
    sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD = 0;
    sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

    DEVICE->CreateSamplerState(&sampDesc, &sampler);

    SetWireFrame(true);
}

TutorialScene::~TutorialScene()
{
    delete vertexBuffer;
    delete indexBuffer;

    delete worldBuffer;
    delete colorBuffer;
    delete lightBuffer;

    srv->Release();
    sampler->Release();
}

void TutorialScene::Update()
{
    static float angle = 0.0f;

    angle += 0.001f;

    Matrix world = XMMatrixRotationRollPitchYaw(0.3f, angle, 0.0f);

    worldBuffer->SetWorld(world);

    if (GetAsyncKeyState(VK_F1))
        SetWireFrame(true);
    if (GetAsyncKeyState(VK_F2))
        SetWireFrame(false);
}

void TutorialScene::PreRender()
{
}

void TutorialScene::Render()
{
    DC->RSSetState(state);

    vertexBuffer->Set();
    indexBuffer->Set();
    IASetPT();

    worldBuffer->SetVSBuffer(1);
    lightBuffer->SetVSBuffer(2);
    colorBuffer->SetPSBuffer(0);

    shader->Set();

    DC->PSSetShaderResources(0, 1, &srv);
    DC->PSSetSamplers(0, 1, &sampler);

    DC->DrawIndexed(36, 0, 0);
}

void TutorialScene::PostRender()
{
    ImGui::ColorEdit4("Color", (float*)&colorBuffer->data.color);
    ImGui::SliderFloat3("LightDir", (float*)&lightBuffer->data.direction, -50.0f, 50.0f);
}

void TutorialScene::SetWireFrame(bool value)
{
    if (state != nullptr)
        state->Release();

    D3D11_RASTERIZER_DESC desc = {};
    desc.AntialiasedLineEnable = false;
    desc.CullMode = D3D11_CULL_BACK;
    if (value)
        desc.FillMode = D3D11_FILL_WIREFRAME;
    else
        desc.FillMode = D3D11_FILL_SOLID;

    DEVICE->CreateRasterizerState(&desc, &state);
}


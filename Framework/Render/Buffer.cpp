#include "Framework.h"
#include "Buffer.h"

VertexBuffer::VertexBuffer(void* data, UINT stride, UINT count, bool isCpuWrite, bool isGpuWrite)
    : stride(stride), offset(0)
{
    D3D11_BUFFER_DESC bd = {};        
    //D3D11_USAGE_DEFAULT = 0,//GPU���� �а� ���� ����
    //D3D11_USAGE_IMMUTABLE = 1,//GPU���� �б⸸ ����(���� �Ұ�)
    //D3D11_USAGE_DYNAMIC = 2,//CPU���� ���Ⱑ��, GPU���� �б�(Map, Unmap ��밡��)
    //D3D11_USAGE_STAGING = 3//GPU���� CPU�� �̵�,(�б⸸ ����)
    if (isCpuWrite == false && isGpuWrite == false)
    {
        bd.Usage = D3D11_USAGE_IMMUTABLE;
    }
    else if (isCpuWrite == true && isGpuWrite == false)
    {
        bd.Usage = D3D11_USAGE_DYNAMIC;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    }
    else if (isCpuWrite == false && isGpuWrite == true)
    {
        bd.Usage = D3D11_USAGE_DEFAULT;
    }
    else
    {
        bd.Usage = D3D11_USAGE_STAGING;
        bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
    }

    bd.ByteWidth = stride * count;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = data;

    V(DEVICE->CreateBuffer(&bd, &initData, &buffer));
}

VertexBuffer::~VertexBuffer()
{
    buffer->Release();
}

void VertexBuffer::Set(UINT slot)
{
    DC->IASetVertexBuffers(slot, 1, &buffer, &stride, &offset);
}

void VertexBuffer::Update(void* data, UINT count)
{
    DC->UpdateSubresource(buffer, 0, nullptr, data, stride, count);
}

///////////////////////////////////////////////////////////////////////////

IndexBuffer::IndexBuffer(void* data, UINT count)
{
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = sizeof(UINT) * count;
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = data;

    V(DEVICE->CreateBuffer(&bd, &initData, &buffer));
}

IndexBuffer::~IndexBuffer()
{
    buffer->Release();
}

void IndexBuffer::Set()
{
    DC->IASetIndexBuffer(buffer, DXGI_FORMAT_R32_UINT, 0);
}

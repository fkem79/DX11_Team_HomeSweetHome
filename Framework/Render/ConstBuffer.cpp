#include "Framework.h"
#include "ConstBuffer.h"

ConstBuffer::ConstBuffer(void* data, UINT dataSize)
	: data(data), dataSize(dataSize)
{
    D3D11_BUFFER_DESC bd = {};
    bd.Usage = D3D11_USAGE_DYNAMIC;
    bd.ByteWidth = dataSize;
    bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    V(DEVICE->CreateBuffer(&bd, nullptr, &buffer));
}

ConstBuffer::~ConstBuffer()
{
    buffer->Release();
}

void ConstBuffer::MapData()
{
    D3D11_MAPPED_SUBRESOURCE subResource;

    DC->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &subResource);
    memcpy(subResource.pData, data, dataSize);
    DC->Unmap(buffer, 0);
}

void ConstBuffer::SetVSBuffer(UINT slot)
{
    MapData();
    DC->VSSetConstantBuffers(slot, 1, &buffer);
}

void ConstBuffer::SetPSBuffer(UINT slot)
{
    MapData();
    DC->PSSetConstantBuffers(slot, 1, &buffer);
}

void ConstBuffer::SetGSBuffer(UINT slot)
{
    MapData();
    DC->GSSetConstantBuffers(slot, 1, &buffer);
}

void ConstBuffer::SetCSBuffer(UINT slot)
{
    MapData();
    DC->CSSetConstantBuffers(slot, 1, &buffer);
}

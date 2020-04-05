#pragma once

inline void IASetPT(D3D_PRIMITIVE_TOPOLOGY type = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
{
	DC->IASetPrimitiveTopology(type);
}

class VertexBuffer
{
private:
	ID3D11Buffer* buffer;

	UINT stride;
	UINT offset;

public:
	VertexBuffer(void* data, UINT stride, UINT count, bool isCpuWrite = false, bool isGpuWrite = false);
	~VertexBuffer();

	void Set(UINT slot = 0);

	void Update(void* data, UINT count);

	ID3D11Buffer* GetBuffer() { return buffer; }
};

////////////////////////////////////////////////////////

class IndexBuffer
{
private:
	ID3D11Buffer* buffer;

public:
	IndexBuffer(void* data, UINT count);
	~IndexBuffer();

	void Set();
};
#pragma once

class RawBuffer : public CsResource
{
private:
	void* inputData;
	UINT byteWidth;

public:
	RawBuffer(void* inputData, UINT byteWidth);
	~RawBuffer();	
	
	void Copy(void* data, UINT size);

	UINT ByteWidth() { return byteWidth; }

private:
	virtual void CreateInput() override;
	virtual void CreateSRV() override;
	virtual void CreateOutput() override;
	virtual void CreateUAV() override;

	virtual void CreateResult() override;	
};

///////////////////////////////////////////////////////

class StructuredBuffer : public CsResource
{
private:
	void* inputData;

	UINT inputStride;
	UINT inputCount;
	
	UINT outputStride;
	UINT outputCount;

public:
	StructuredBuffer(void* inputData, UINT inputStride, UINT inputCount,
		UINT outputStride = 0, UINT outputCount = 0);
	~StructuredBuffer();

	void Copy(void* data, UINT size);

	UINT InputByteWidth() { return inputStride * inputCount; }
	UINT OutputByteWidth() { return outputStride * outputCount; }

private:
	virtual void CreateInput() override;
	virtual void CreateSRV() override;
	virtual void CreateOutput() override;
	virtual void CreateUAV() override;

	virtual void CreateResult() override;
};

///////////////////////////////////////////////////////////////////

class TextureBuffer : public CsResource
{
private:
	UINT width, height, page;
	DXGI_FORMAT format;

	ID3D11Texture2D* inputTexture;
	ID3D11ShaderResourceView* outputSrv;

public:
	TextureBuffer(ID3D11Texture2D* src);
	~TextureBuffer();

	UINT Width() { return width; }
	UINT Height() { return height; }
	UINT Page() { return page; }

	ID3D11Texture2D* Input() { return (ID3D11Texture2D*)input; }
	ID3D11Texture2D* Output() { return (ID3D11Texture2D*)output; }
	ID3D11ShaderResourceView* OutputSrv() { return outputSrv; }
private:
	virtual void CreateInput() override;
	virtual void CreateSRV() override;
	virtual void CreateOutput() override;
	virtual void CreateUAV() override;

	virtual void CreateResult() override;
};

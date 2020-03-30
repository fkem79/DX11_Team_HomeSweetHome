#include "Framework.h"
#include "ComputShader.h"

ComputeShader::ComputeShader(wstring file)
{
	this->file = L"Shaders/" + file + L".hlsl";

	CreateShader();
}

ComputeShader::~ComputeShader()
{
	shader->Release();
}

void ComputeShader::Set()
{
	DC->CSSetShader(shader, nullptr, 0);
}

void ComputeShader::CreateShader()
{	
	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

	ID3DBlob* blob;

	V(D3DCompileFromFile(file.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"CS", "cs_5_0", flags, 0, &blob, nullptr));

	V(DEVICE->CreateComputeShader(blob->GetBufferPointer(), blob->GetBufferSize(),
		nullptr, &shader));

	blob->Release();
}

#include "Framework.h"
#include "Shader.h"

map<wstring, Shader*> Shader::totalShader;

Shader::Shader(wstring file, string vsName, string psName)
	: vsName(vsName), psName(psName), gsName(""),
	geometryShader(nullptr), geometryBlob(nullptr)
{
	shaderFile = L"Shaders/" + file + L".hlsl";

	CreateVertexShader();
	CreatePixelShader();
	CreateInputLayout();
}

Shader::~Shader()
{
	vertexShader->Release();
	pixelShader->Release();
	inputLayout->Release();

	vertexBlob->Release();
	pixelBlob->Release();

	if (geometryShader != nullptr)
		geometryShader->Release();
	if (geometryBlob != nullptr)
		geometryBlob->Release();
}

Shader* Shader::Add(wstring file, string vsName, string psName)
{
	wstring key = file + Path::ToWString(vsName) + Path::ToWString(psName);
	if (totalShader.count(key) > 0)
		return totalShader[key];
	else
	{
		totalShader.insert({ key, new Shader(file, vsName, psName) });
		return totalShader[key];
	}
}

void Shader::Delete()
{
	for (auto shader : totalShader)
		delete shader.second;
}

void Shader::Set()
{
	DC->IASetInputLayout(inputLayout);
	DC->VSSetShader(vertexShader, nullptr, 0);
	DC->PSSetShader(pixelShader, nullptr, 0);
	if (geometryShader != nullptr)
		DC->GSSetShader(geometryShader, nullptr, 0);
}

void Shader::CreateGeometryShader(string gsName)
{
	if (geometryShader != nullptr)
		return;

	this->gsName = gsName;

	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

	V(D3DCompileFromFile(shaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		gsName.c_str(), "gs_5_0", flags, 0, &geometryBlob, nullptr));
	V(DEVICE->CreateGeometryShader(geometryBlob->GetBufferPointer(), geometryBlob->GetBufferSize(),
		nullptr, &geometryShader));
}

void Shader::CreateVertexShader()
{
	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
	
	V(D3DCompileFromFile(shaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		vsName.c_str(), "vs_5_0", flags, 0, &vertexBlob, nullptr));
	V(DEVICE->CreateVertexShader(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(),
		nullptr, &vertexShader));
}

void Shader::CreatePixelShader()
{
	DWORD flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;

	V(D3DCompileFromFile(shaderFile.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
		psName.c_str(), "ps_5_0", flags, 0, &pixelBlob, nullptr));
	V(DEVICE->CreatePixelShader(pixelBlob->GetBufferPointer(), pixelBlob->GetBufferSize(),
		nullptr, &pixelShader));
}

void Shader::CreateInputLayout()
{
	V(D3DReflect(vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(),
		IID_ID3D11ShaderReflection, (void**)&reflection));

	D3D11_SHADER_DESC shaderDesc;
	reflection->GetDesc(&shaderDesc);

	vector<D3D11_INPUT_ELEMENT_DESC> inputLayouts;

	for (UINT i = 0; i < shaderDesc.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		reflection->GetInputParameterDesc(i, &paramDesc);

		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask < 4)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask < 8)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask < 16)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
				elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		string temp = paramDesc.SemanticName;
		if (temp == "POSITION")
			elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;

		if (temp == "INSTANCE")
		{
			elementDesc.InputSlot = 1;
			elementDesc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
			elementDesc.InstanceDataStepRate = 1;
		}

		inputLayouts.push_back(elementDesc);
	}

	V(DEVICE->CreateInputLayout(inputLayouts.data(), inputLayouts.size(),
		vertexBlob->GetBufferPointer(), vertexBlob->GetBufferSize(), &inputLayout));
}

#pragma once

class Shader
{
private:
	wstring shaderFile;
	string vsName;
	string psName;
	string gsName;

	ID3D11VertexShader* vertexShader;
	ID3D11PixelShader* pixelShader;
	ID3D11GeometryShader* geometryShader;

	ID3D11InputLayout* inputLayout;

	ID3DBlob* vertexBlob;
	ID3DBlob* pixelBlob;
	ID3DBlob* geometryBlob;

	ID3D11ShaderReflection* reflection;

	static map<wstring, Shader*> totalShader;

	Shader(wstring file, string vsName, string psName);
	~Shader();
public:
	static Shader* Add(wstring file, string vsName = "VS", string psName = "PS");
	static void Delete();

	void Set();

	void CreateGeometryShader(string gsName = "GS");
private:
	void CreateVertexShader();
	void CreatePixelShader();	

	void CreateInputLayout();
};
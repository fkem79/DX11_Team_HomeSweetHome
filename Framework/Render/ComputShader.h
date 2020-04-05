#pragma once

class ComputeShader
{
private:
	wstring file;

	ID3D11ComputeShader* shader;

public:
	ComputeShader(wstring file);
	~ComputeShader();

	void Set();

private:
	void CreateShader();
};
#pragma once

class Render2D : public Transform
{
protected:
	Shader* shader;
	ID3D11ShaderResourceView* srv;

	VertexBuffer* vertexBuffer;

	Matrix view;
	Matrix orthographic;

	DepthStencilState* depthState;

public:
	Render2D(wstring shaderFile = L"Render2D");
	virtual ~Render2D();

	virtual void Update();
	virtual void Render();

	void SetSRV(ID3D11ShaderResourceView* value) { srv = value; }
	

}; 
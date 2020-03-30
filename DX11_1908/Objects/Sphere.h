#pragma once

class Sphere : public Transform
{
private:
	typedef VertexUVNormalTangent VertexType;

	Material* material;

	vector<VertexType> vertices;
	vector<UINT> indices;
	
	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	IntBuffer* intBuffer;

	UINT sliceCount;
	UINT stackCount;	
	float radius;

	RasterizerState* fillMode[2];
public:
	Sphere(float radius = 1.0f, UINT sliceCount = 20, UINT stackCount = 10);
	Sphere(wstring shaderFile, float radius = 1.0f, UINT sliceCount = 20, UINT stackCount = 10);
	~Sphere();

	void Update();
	void Render();

	void CreateData();
	void CreateTangent();

	float GetRadius() { return radius; }
	Material* GetMaterial() { return material; }
	RasterizerState* GetFillMode() { return fillMode[1]; }
};
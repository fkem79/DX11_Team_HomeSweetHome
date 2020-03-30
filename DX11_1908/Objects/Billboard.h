#pragma once

class Billboard : public Transform
{
private:
	Material* material;

	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

public:
	Billboard(wstring diffuseFile);
	~Billboard();

	void Update();
	void Render();

	void CreateData();
};
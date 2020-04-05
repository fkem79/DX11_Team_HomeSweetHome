#pragma once

struct Ray
{
	Vector3 position;
	Vector3 direction;
};

class Collider : public Transform
{
protected:
	Shader* shader;

	ColorBuffer* colorBuffer;

	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	vector<Vertex> vertices;
	vector<UINT> indices;	
public:
	Collider();
	virtual ~Collider();

	virtual bool IsCollision(Collider* collider) = 0;
	virtual bool IsCollision(IN Ray ray, OUT float* distance = nullptr,
		OUT Vector3* contact = nullptr) = 0;
	
	void Render();

	void SetColor(Float4 color) { colorBuffer->data.color = color; }
};
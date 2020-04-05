#pragma once

class InstancingScene : public Scene
{
private:
	const static UINT COUNT = 1000;

	Material* material;

	vector<VertexUV> vertices;
	vector<UINT> indices;

	VertexBuffer* vertexBuffer;
	VertexBuffer* instanceBuffer;
	IndexBuffer* indexBuffer;

	ID3D11Buffer* vertexBuffers[2];

	WorldBuffer* worldBuffer;

	Matrix worlds[COUNT];
public:
	InstancingScene();
	~InstancingScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	void CreateData();
};
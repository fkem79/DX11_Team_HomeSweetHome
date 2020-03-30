#pragma once

class GridScene : public Scene
{
private:
	typedef VertexUVNormal VertexType;

	Shader* shader;

	WorldBuffer* worldBuffer;

	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	vector<VertexType> vertices;
	vector<UINT> indices;

	UINT width, height;

	ID3D11RasterizerState* state;

	Texture* texture;
	Texture* heightMap;
public:
	GridScene();
	~GridScene();
	
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	void CreateData();
	void CreateNormal();
};
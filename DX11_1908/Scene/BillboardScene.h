#pragma once

class Billboard;

class BillboardScene : public Scene
{
private:
	Terrain* terrain;

	//vector<Billboard*> trees;

	Shader* shader;
	VertexBuffer* vertexBuffer;
	UINT count;
	Texture* texture;

	BlendState* blendMode[2];
public:
	BillboardScene();
	~BillboardScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};
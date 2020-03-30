#pragma once

class TutorialScene : public Scene
{
private:
	Shader* shader;

	VertexBuffer* vertexBuffer;
	IndexBuffer* indexBuffer;

	WorldBuffer* worldBuffer;

	ColorBuffer* colorBuffer;
	LightBuffer* lightBuffer;

	ID3D11ShaderResourceView* srv;
	ID3D11SamplerState* sampler;
		
	ID3D11RasterizerState* state;
public:
	TutorialScene();
	~TutorialScene();
	
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	void SetWireFrame(bool value);
};
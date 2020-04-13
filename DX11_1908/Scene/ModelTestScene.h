#pragma once

class ModelTestScene : public Scene
{
private:
	ModelSingle* model;

public:
	ModelTestScene();
	~ModelTestScene();
	
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};

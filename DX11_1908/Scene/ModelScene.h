#pragma once

class ModelScene : public Scene
{
private:
	Kaya* kaya;

	//Model* model;
	//ModelRender* modelRender;

	ModelAnimator* animator;
public:
	ModelScene();
	~ModelScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};
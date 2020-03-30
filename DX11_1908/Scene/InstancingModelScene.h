#pragma once

class InstancingModelScene : public Scene
{
private:
	//ModelRender* kaya;
	ModelAnimator* kaya;
public:
	InstancingModelScene();
	~InstancingModelScene();
	
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	void Kaya();
};
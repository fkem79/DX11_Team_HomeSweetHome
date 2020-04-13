#pragma once

class WaterScene : public Scene
{
private:
	Water* water;
	Sphere* sphere;
	CubeSky* sky;

public:
	WaterScene();
	~WaterScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};
#pragma once
class FlashLight
{
private:
	ModelRender* flashLight;

public:
	FlashLight();
	~FlashLight();

	void Update();
	void Render();

	void CreateFlashLight();

	ModelRender* GetFlashLight() { return flashLight; }
};

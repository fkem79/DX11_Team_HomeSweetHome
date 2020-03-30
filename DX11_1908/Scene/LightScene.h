#pragma once

class LightScene : public Scene
{
private:
	ModelRender* bunny;
	ModelRender* plane;

	LightInfoBuffer* buffer;
public:
	LightScene();
	~LightScene();
		
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	void Export(string name);
};
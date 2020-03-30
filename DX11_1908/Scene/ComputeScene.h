#pragma once

class ComputeScene : public Scene
{
private:
	Render2D* render2D;
public:
	ComputeScene();
	~ComputeScene();
	
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	void Raw();
	void Intersection();
	void Texture2D();
};
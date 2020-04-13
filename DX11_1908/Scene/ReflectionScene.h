#pragma once

class ReflectionScene : public Scene
{
private:
	CubeSky* sky;
	Sphere* sphere;	
	Terrain* terrain;
	
	Reflection* reflection;
	Refraction* refraction;
public:
	ReflectionScene();
	~ReflectionScene();
	
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};
#pragma once

class LandscapeScene : public Scene
{
private:
	Sphere* sphere;
	Terrain* terrain;
public:
	LandscapeScene();
	~LandscapeScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};
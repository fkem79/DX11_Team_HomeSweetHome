#pragma once

class ShadowScene : public Scene
{
private:
	Terrain* terrain;
	Kaya* kaya;	

	Shadow* shadow;	
public:
	ShadowScene();
	~ShadowScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};
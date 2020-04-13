#pragma once
class MapScene : public Scene
{
private:
	Belle* belle;
	Player* player;
	ModelSingle* tile1;

	float objectMatrix[16] =
	{ 1.f, 0.f, 0.f, 0.f,
	  0.f, 1.f, 0.f, 0.f,
	  0.f, 0.f, 1.f, 0.f,
	  0.f, 0.f, 0.f, 1.f };

public:

	MapScene();
	~MapScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	void GizmoTest();
};

#pragma once

struct ObjData
{
	Float3 Position;
	Float3 Rotation;
	Float3 Scale;

	Float3 min;
	Float3 max;
};


class MapScene : public Scene
{
private:
	Belle* belle;
	Player* player;
	ModelSingle* tile1;
	vector<ModelSingle*> totalObj;

	float totalObjTestX;

	float objectMatrix[16] =
	{ 1.f, 0.f, 0.f, 0.f,
	  0.f, 1.f, 0.f, 0.f,
	  0.f, 0.f, 1.f, 0.f,
	  0.f, 0.f, 0.f, 1.f };

	bool mapToolWindow;
	bool check;

public:

	MapScene();
	~MapScene();

	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	void GizmoTest();
	void ImGuiSaveLoadTest();
};

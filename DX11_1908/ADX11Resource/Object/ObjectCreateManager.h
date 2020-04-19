#pragma once

struct ObjData
{
	UINT objNum;
	UINT objNameNum;

	Float3 Position;
	Float3 Rotation;
	Float3 Scale;

	Float3 collPos;
	Float3 collRot;
	Float3 collScale;
};

class ObjectCreateManager
{
private:

	vector<string> objNames;

	vector<ModelSingle*> totalObj;
	
	UINT curObjNum;
	UINT objNum;

	char curStr[20];

	float totalObjTestX;

	bool mapToolWindow;
	bool addNameWindow;

	bool check;
	bool fileCheck;

	bool allObjBoxRenderOn;

public:
	ObjectCreateManager();
	~ObjectCreateManager();

	void AddObject(ModelSingle* addObj);
	void Update();
	void Render();

	void PostRender();

	void ObjectCreateWindow();
	void ObjectSettingWindow();

	UINT FindNameNum(string fineName);

	void AddNames();
};

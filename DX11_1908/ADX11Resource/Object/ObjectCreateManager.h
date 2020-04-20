#pragma once
/*
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
*/
struct ObjData
{
	string objName;
	UINT objNum;

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
	vector<ModelSingle*> totalObj;
	vector<UINT> emptyNums;
	
	UINT curObjNum;
	UINT totalObjNum;

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

	void Update();
	void Render();

	void PostRender();

	void ObjectCreateWindow();
	void ObjectSettingWindow();

	void MapToolAddWindow();
	void MapToolSameAdd();
	void MapToolCopy();
	void MapToolDelete();
	void MapToolSave();
	void MapToolLoad();

};

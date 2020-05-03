//#pragma once
//
//struct ObjData
//{
//	string objName;
//	UINT objNum;
//	string objShader;
//
//	Float3 Position;
//	Float3 Rotation;
//	Float3 Scale;
//};
//
//class ObjectCreateManager
//{
//private:
//	
//	int shaderMode;
//
//	vector<ModelSingle*> totalObj;
//	vector<string> loadCompleteList;
//	
//	int curObjIndex;
//	UINT totalObjNum;
//
//	char curFileName[20];
//	char saveLoadName[20];
//
//	float addXPos, addZPos;
//
//	bool mapToolWindow;
//	bool addNameWindow;
//	bool saveNameWindow;
//	bool loadNameWindow;
//
//	bool check;
//	bool fileCheck;
//
//	bool allObjBoxRenderOn;
//
//	Frustum* frustum;
//
//public:
//
//	ObjectCreateManager();
//	~ObjectCreateManager();
//
//	void Update();
//	void Render();
//
//	void PostRender();
//
//	void ObjectCreateWindow();
//	void ObjectSettingWindow();
//
//	void MapToolAddWindow();
//	void MapToolSameAdd();
//	void MapToolCopy();
//	void MapToolDelete();
//	void MapToolSave();
//	void MapToolLoad();
//
//};
#pragma once

struct ObjData
{
	string objName;
	UINT objNum;
	string objShader;

	Float3 Position;
	Float3 Rotation;
	Float3 Scale;
};

class ObjectCreateManager
{
private:

	int shaderMode;

	//vector<ModelSingle*> totalObj;
	vector<ModelRender*> insTotalObj;
	vector<string> loadCompleteList;

	int curObjIndex;
	UINT totalObjNum;

	char curFileName[20];
	char saveLoadName[20];

	float addXPos, addZPos;

	bool mapToolWindow;
	bool addNameWindow;
	bool saveNameWindow;
	bool loadNameWindow;

	bool check;
	bool fileCheck;

	bool allObjBoxRenderOn;

	Frustum* frustum;
	string curModelName;

	UINT Num1;
	UINT Num2;

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
	//void MapToolCopy();
	//void MapToolDelete();
	//void MapToolSave();
	//void MapToolLoad();

	int SerachModelName(string name);
	void SetInstanceNum(UINT value) { Num2 = value; }
};

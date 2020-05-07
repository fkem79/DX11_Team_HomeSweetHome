#pragma once

class BoxCollider;

class ModelSingle : public Transform
{
private:
	UINT modelNum;
	Model* model;
	BoxCollider* coll;

	string modelName;
	string shaderName;

	bool check;
	bool boxRenderCheck;

	Float3 boxMin = { 0,0,0 };
	Float3 boxMax = { 0,0,0 };
	int meshSize;
	int countFor = 0;

	Float3 one, two;

public:
	ModelSingle(string name);
	ModelSingle(string name, string shaderName);
	~ModelSingle();

	void Update();
	void Render();
	void PostRender();

	void Export(string name);
	void SetCheck(bool value) { check = value; }

	BoxCollider* GetCollBox() { return coll; }
	string GetModelName() { return modelName; }
	string GetShaderName() { return shaderName; }
	UINT GetModelNum() { return modelNum; }
	bool GetBoxRenderCheck() { return boxRenderCheck; }
	

	void SetModelShader(string shaderName) { model->SetShader(Path::ToWString(shaderName)); }
	void SetBoxRenderCheck(bool value) { boxRenderCheck = value; }
	void SetModelNum(UINT value) { modelNum = value; }

	void CreateCollBox();

};
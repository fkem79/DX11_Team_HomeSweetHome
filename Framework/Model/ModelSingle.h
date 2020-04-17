#pragma once

class BoxCollider;

class ModelSingle : public Transform
{
private:
	UINT modelNum;
	Model* model;
	BoxCollider* coll;
	string modelName;

	bool check;
	bool boxRenderCheck;

public:
	ModelSingle(string name);
	~ModelSingle();

	void Update();
	void Render();
	void PostRender();

	void Export(string name);
	void SetCheck(bool value) { check = value; }
	BoxCollider* GetCollBox() { return coll; }
	string GetModelName() { return modelName; }
	UINT GetModelNum() { return modelNum; }

	void SetBoxRenderCheck(bool value) { boxRenderCheck = value; }
	void SetModelNum(UINT value) { modelNum = value; }

};
#pragma once

class BoxCollider;

class ModelSingle : public Transform
{
private:
	Model* model;
	BoxCollider* coll;

	bool check;
public:
	ModelSingle(string name);
	~ModelSingle();

	void Update();
	void Render();
	void PostRender();

	void Export(string name);
	void SetCheck(bool value) { check = value; }
	BoxCollider* GetCollBox() { return coll; }
};
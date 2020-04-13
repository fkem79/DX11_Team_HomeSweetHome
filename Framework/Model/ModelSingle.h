#pragma once

class ModelSingle : public Transform
{
private:
	Model* model;
public:
	ModelSingle(string name);
	~ModelSingle();

	void Update();
	void Render();

	void Export(string name);
};
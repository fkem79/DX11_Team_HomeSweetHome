#pragma once

class Program
{
private:
	vector<Scene*> scenes;

public:
	Program();
	~Program();

	void Update();

	void PreRender();
	void Render();
	void PostRender();
};
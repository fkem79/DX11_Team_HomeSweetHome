#pragma once

class SceneManager
{
private:
	SceneManager();
	~SceneManager();
public:
	static SceneManager* Get();
	static void Create();
	static void Delete();

	void Update();
	void PreRender();
	void Render();
	void PostRender();

	Scene* Add(string key, Scene* scene);

	void ChangeScene(string key);
private:
	static SceneManager* instance;

	map<string, Scene*> totalScene;
	Scene* curScene;

};
#include "Framework.h"

SceneManager* SceneManager::instance = nullptr;

SceneManager::SceneManager() : curScene(nullptr)
{
}

SceneManager::~SceneManager()
{
	for (auto scene : totalScene)
		delete scene.second;
}

SceneManager* SceneManager::Get()
{
	return instance;
}

void SceneManager::Create()
{
	instance = new SceneManager();
}

void SceneManager::Delete()
{
	delete instance;
}

void SceneManager::Update()
{
	if (curScene == nullptr)
		return;

	curScene->Update();
}

void SceneManager::PreRender()
{
	if (curScene == nullptr)
		return;

	curScene->PreRender();
}

void SceneManager::Render()
{
	if (curScene == nullptr)
		return;

	curScene->Render();
}

void SceneManager::PostRender()
{
	if (curScene == nullptr)
		return;

	curScene->PostRender();
}

Scene* SceneManager::Add(string key, Scene* scene)
{
	if (totalScene.count(key) > 0)
		return totalScene[key];

	totalScene.insert({ key, scene });

	return scene;
}

void SceneManager::ChangeScene(string key)
{
	if (totalScene.count(key) == 0)
		return;

	if (totalScene[key] == curScene)
		return;

	if (curScene != nullptr)
		curScene->End();

	curScene = totalScene[key];
	curScene->Begin();
}

#include "Framework.h"
#include "Program.h"

//#include "Scene/TutorialScene.h"
//#include "Scene/GridScene.h"
#include "Scene/LandscapeScene.h"
#include "Scene/ModelScene.h"
#include "Scene/BillboardScene.h"
#include "Scene/ParticleScene.h"
#include "Scene/InstancingScene.h"
#include "Scene/InstancingModelScene.h"
#include "Scene/ComputeScene.h"
#include "Scene/CollisionScene.h"
#include "Scene/RenderTargetScene.h"
#include "Scene/ShadowScene.h"
#include "Scene/LightScene.h"
#include "Scene/AStarScene.h"
#include "Scene/ReflectionScene.h"

Program::Program()
{
	//scenes.push_back(new TutorialScene());
	//scenes.push_back(new GridScene());
	//scenes.push_back(new LandscapeScene());
	//scenes.push_back(new ModelScene());
	//scenes.push_back(new BillboardScene());
	//scenes.push_back(new ParticleScene());
	//scenes.push_back(new InstancingScene());
	//scenes.push_back(new InstancingModelScene());
	//scenes.push_back(new ComputeScene());
	//scenes.push_back(new CollisionScene());
	//scenes.push_back(new RenderTargetScene());
	//scenes.push_back(new ShadowScene());
	//scenes.push_back(new LightScene());
	//scenes.push_back(new AStarScene());
	scenes.push_back(new ReflectionScene());
}

Program::~Program()
{
	for (Scene* scene : scenes)
		delete scene;
}

void Program::Update()
{
	for (Scene* scene : scenes)
		scene->Update();

	CAMERA->Update();	
	Keyboard::Get()->SetWheel(0.0f);
}

void Program::PreRender()
{
	VP->SetProjection(Environment::Get()->GetProjection());	
	VP->SetView(CAMERA->GetView());
	VP->SetVSBuffer(0);
	LIGHT->SetPSBuffer(0);

	for (Scene* scene : scenes)
		scene->PreRender();
}

void Program::Render()
{
	VP->SetProjection(Environment::Get()->GetProjection());
	VP->SetView(CAMERA->GetView());
	VP->SetVSBuffer(0);

	for (Scene* scene : scenes)
		scene->Render();
}

void Program::PostRender()
{
	ImGui::Text("FPS : %d", (int)Timer::Get()->FPS());
	ImGui::Checkbox("Vsync", Device::Get()->Vsync());
	CAMERA->PostRender();
	ImGui::Text("Light");
	ImGui::SliderFloat3("LightDir", (float*)&LIGHT->data.direction, -1, 1);
	ImGui::SliderFloat("SpecExp", &LIGHT->data.specExp, 1.0f, 100.0f);
	ImGui::ColorEdit4("Ambient", (float*)&LIGHT->data.ambient);
	ImGui::ColorEdit4("AmbientFloor", (float*)&LIGHT->data.ambientFloor);

	for (Scene* scene : scenes)
		scene->PostRender();
}

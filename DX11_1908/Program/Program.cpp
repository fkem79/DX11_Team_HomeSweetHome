#include "Framework.h"
#include "Program.h"

#include "Scene/MapScene.h"
#include "Scene/StartScene.h"


Program::Program()
{
	LIGHT->data.ambient = { 1.0f, 1.0f, 1.0f, 0.0f };

	SCENE->Add("start", new StartScene());
	SCENE->Add("play", new MapScene());

	SCENE->ChangeScene("start");
}

Program::~Program()
{
}

void Program::Update()
{
	SOUND->Update();

	SCENE->Update();

	CAMERA->Update();	
	Keyboard::Get()->SetWheel(0.0f);
}

void Program::PreRender()
{
	VP->SetProjection(Environment::Get()->GetProjection());	
	VP->SetView(CAMERA->GetView());
	VP->SetVSBuffer(0);
	LIGHT->SetPSBuffer(0);

	SCENE->PreRender();
}

void Program::Render()
{
	VP->SetProjection(Environment::Get()->GetProjection());
	VP->SetView(CAMERA->GetView());
	VP->SetVSBuffer(0);

	SCENE->Render();
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

	SCENE->PostRender();
}

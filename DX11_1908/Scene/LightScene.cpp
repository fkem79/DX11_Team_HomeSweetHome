#include "Framework.h"
#include "LightScene.h"

LightScene::LightScene()
{
	string name = "Bunny";

	//Export(name);
	bunny = new ModelRender(L"Lighting");
	bunny->ReadMaterial(name + "/" + name);
	bunny->ReadMesh(name + "/" + name);
	bunny->GetModel()->SetDiffuseMap(L"Textures/White.png");
	Transform* transform = bunny->AddTransform();
	transform->scale = Vector3(0.01f, 0.01f, 0.01f);
	transform->position.SetY(2.3f);

	name = "Plane";
	//Export(name);
	plane = new ModelRender(L"Lighting");
	plane->ReadMaterial(name + "/" + name);
	plane->ReadMesh(name + "/" + name);
	plane->AddTransform()->scale = Vector3(30, 30, 30);
	plane->GetModel()->SetDiffuseMap(L"Textures/Landscape/Dirt2.png");

	buffer = new LightInfoBuffer();
	LightInfo info;
	info.type = LightInfo::POINT;
	info.color = Float4(1, 0, 0, 1);
	info.position = Float3(-5, 5, 0);

	buffer->Add(info);

	info.color = Float4(0, 1, 0, 1);
	info.position = Float3(0, 5, 0);

	buffer->Add(info);

	info.color = Float4(0, 0, 1, 1);
	info.position = Float3(5, 5, 0);

	buffer->Add(info);
}

LightScene::~LightScene()
{
	delete bunny;
	delete plane;

	delete buffer;
}

void LightScene::Update()
{
	bunny->Update();
	plane->Update();
}

void LightScene::PreRender()
{
}

void LightScene::Render()
{
	buffer->SetPSBuffer(3);

	bunny->Render();
	plane->Render();
}

void LightScene::PostRender()
{
	ImGui::Text("LightInfo");
	for (UINT i = 0; i < buffer->data.lightCount; i++)
	{
		ImGui::SliderInt("Type" + i, (int*)&buffer->data.lights[i].type, 0, 3);
		ImGui::ColorEdit4("Color" + i, (float*)&buffer->data.lights[i].color);
		ImGui::SliderFloat3("Position" + i, (float*)&buffer->data.lights[i].position, -100, 100);
		ImGui::SliderFloat("Range" + i, &buffer->data.lights[i].range, 0, 100);
		ImGui::SliderFloat3("Direction" + i, (float*)&buffer->data.lights[i].direction, -1, 1);
		ImGui::SliderFloat("Outer" + i, &buffer->data.lights[i].outer, 0, 180);
		ImGui::SliderFloat("Inner" + i, &buffer->data.lights[i].inner, 0, 180);
		ImGui::SliderFloat("Length" + i, &buffer->data.lights[i].length, 0, 100);
	}
}

void LightScene::Export(string name)
{
	ModelReader* reader = new ModelReader();
	reader->ReadFile("ModelData/Models/" + name + ".fbx");
	reader->ExportMaterial(name + "/" + name);
	reader->ExportMesh(name + "/" + name);
	delete reader;
}

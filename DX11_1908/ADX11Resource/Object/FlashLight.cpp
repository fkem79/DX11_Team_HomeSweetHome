#include "Framework.h"
#include "FlashLight.h"

FlashLight::FlashLight()
{
	CreateFlashLight();
}

FlashLight::~FlashLight()
{
	delete flashLight;
}

void FlashLight::Update()
{
	flashLight->Update();
}

void FlashLight::Render()
{
	flashLight->Render();
}

void FlashLight::CreateFlashLight()
{
	string name = "flashLight";

	ModelReader* reader = new ModelReader();
	reader->ReadFile("ModelData/Models/" + name + ".fbx");
	reader->ExportMaterial(name + "/" + name);
	reader->ExportMesh(name + "/" + name);
	delete reader;

	flashLight = new ModelRender(L"ModelInstancing");
	flashLight->ReadMaterial(name + "/" + name);
	flashLight->ReadMesh(name + "/" + name);

	Transform* trans = flashLight->AddTransform();
	trans->scale = { 1.0f, 1.0f, 1.0f };
	trans->rotation = { 1.57f,0,0 };
	trans->position = { 10,0,0 };
	trans->UpdateWorld();

	flashLight->UpdateTransforms();
}

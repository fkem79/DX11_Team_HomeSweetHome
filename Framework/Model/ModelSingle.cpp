#include "Framework.h"

ModelSingle::ModelSingle(string name)
	:check(false), modelName(name), boxRenderCheck(true), modelNum(0)
{
	Export(name);

	model = new Model();
	model->ReadMaterial(name + "/" + name);
	model->ReadMesh(name + "/" + name);

	model->SetShader(L"ModelSingle");

	Float3 min = { -40, -40, -40 };
	Float3 max = { 40, 40, 40 };

	coll = new BoxCollider(min, max);
	coll->position = { 0,0,-45 };
}

ModelSingle::~ModelSingle()
{
	delete model;
	delete coll;
}

void ModelSingle::Update()
{
	coll->SetParent(this->GetWorldPointer());
	coll->UpdateWorld();

	UpdateWorld();
}

void ModelSingle::Render()
{
	if(boxRenderCheck)
		coll->Render();

	worldBuffer->SetVSBuffer(1);

	for (ModelMesh* mesh : *model->GetMeshes())
		mesh->Render(1);
}

void ModelSingle::PostRender()
{
	//if (check)
	/*{
		ImGui::Text("CollBox");
		ImGui::InputFloat3("Position", coll->position.data.m128_f32, 3);
		ImGui::InputFloat3("Scale", coll->scale.data.m128_f32, 3);
		ImGui::InputFloat3("Rotation", coll->rotation.data.m128_f32, 3);

		ImGui::InputFloat3("O Position", this->position.data.m128_f32, 3);
		ImGui::InputFloat3("O Scale", this->scale.data.m128_f32, 3);
		ImGui::InputFloat3("O Rotation", this->rotation.data.m128_f32, 3);

		ImGui::InputFloat3("Min", coll->GetMinBox().data.m128_f32, 3);
		ImGui::InputFloat3("Max", coll->GetMaxBox().data.m128_f32, 3);
	}*/
}

void ModelSingle::Export(string name)
{
	ModelReader* reader = new ModelReader();
	reader->ReadFile("ModelData/Models/" + name + ".fbx");
	reader->ExportMaterial(name + "/" + name);
	reader->ExportMesh(name + "/" + name);
	delete reader;
}


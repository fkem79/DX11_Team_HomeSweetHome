#include "Framework.h"

ModelSingle::ModelSingle(string name)
{
	Export(name);

	model = new Model();
	model->ReadMaterial(name + "/" + name);
	model->ReadMesh(name + "/" + name);

	model->SetShader(L"ModelSingle");
}

ModelSingle::~ModelSingle()
{
	delete model;
}

void ModelSingle::Update()
{
	UpdateWorld();
}

void ModelSingle::Render()
{
	worldBuffer->SetVSBuffer(1);

	for (ModelMesh* mesh : *model->GetMeshes())
		mesh->Render(1);
}

void ModelSingle::Export(string name)
{
	ModelReader* reader = new ModelReader();
	reader->ReadFile("ModelData/Models/" + name + ".fbx");
	reader->ExportMaterial(name + "/" + name);
	reader->ExportMesh(name + "/" + name);
	delete reader;
}


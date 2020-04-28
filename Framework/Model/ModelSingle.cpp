#include "Framework.h"

ModelSingle::ModelSingle(string name)
	:check(false), modelName(name), boxRenderCheck(true), modelNum(0)
{
	Export(name);

	model = new Model();
	model->ReadMaterial(name + "/" + name);
	model->ReadMesh(name + "/" + name);

	shaderName = "ModelSingle";
	model->SetShader(L"ModelSingle");
	//model->SetShader(L"ModelInstancing");

	CreateCollBox();

	for (UINT i = 0; i < MAX_MODEL_INSTANCE; i++)
		worlds[i] = XMMatrixIdentity();

	instanceBuffer = new VertexBuffer(worlds, sizeof(Matrix), MAX_MODEL_INSTANCE, true);
}

ModelSingle::ModelSingle(string name, string shaderName)
	:check(false), modelName(name), boxRenderCheck(true), modelNum(0), shaderName(shaderName)
{
	Export(name);

	model = new Model();
	model->ReadMaterial(name + "/" + name);
	model->ReadMesh(name + "/" + name);

	model->SetShader(Path::ToWString(shaderName));
	//model->SetShader(L"ModelInstancing");

	CreateCollBox();
}

ModelSingle::~ModelSingle()
{
	delete model;
	delete coll;

	delete instanceBuffer;
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

	instanceBuffer->Set(1);

	worldBuffer->SetVSBuffer(1);

	for (ModelMesh* mesh : *model->GetMeshes())
		mesh->Render(1);
}

void ModelSingle::PostRender()
{
}

void ModelSingle::Export(string name)
{
	ModelReader* reader = new ModelReader();
	reader->ReadFile("ModelData/Models/" + name + ".fbx");
	reader->ExportMaterial(name + "/" + name);
	reader->ExportMesh(name + "/" + name);
	delete reader;
}

void ModelSingle::CreateCollBox()
{
	Float3 min = { 0, 0, 0 };
	Float3 max = { 0, 0, 0 };	

	for (ModelMesh* mesh : *model->GetMeshes())
	{
		for (int i = 0; i < mesh->GetMeshVertexCount(); i++)
		{
			ModelVertex* vertices = mesh->GetModelVertexInfo();

			if (min.x > vertices[i].position.x)
				min.x = vertices[i].position.x;
			if (max.x < vertices[i].position.x)
				max.x = vertices[i].position.x;

			if (min.y > vertices[i].position.y)
				min.y = vertices[i].position.y;
			if (max.y < vertices[i].position.y)
				max.y = vertices[i].position.y;

			if (min.z > vertices[i].position.z)
				min.z = vertices[i].position.z;
			if (max.z < vertices[i].position.z)
				max.z = vertices[i].position.z;
		}
	}

	coll = new BoxCollider(min, max);
}


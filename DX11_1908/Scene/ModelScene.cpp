#include "Framework.h"
#include "ModelScene.h"

ModelScene::ModelScene()
{
	//kaya = new Kaya();

	string name = "Brook";

	
	ModelReader* reader = new ModelReader();
	reader->ReadFile("ModelData/Models/" + name + ".fbx");
	reader->ExportMaterial(name + "/" + name);
	reader->ExportMesh(name + "/" + name);	
	delete reader;
	
	reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/" + name + "/Run.fbx");
	reader->ExportAnimClip(0, name + "/Run");
	delete reader;
	/*
	model = new Model();
	model->ReadMaterial(name + "/" + name);
	model->ReadMesh(name + "/" + name);

	modelRender = new ModelRender(L"Model", model);
	*/

	animator = new ModelAnimator(L"ModelAnimation");
	animator->ReadMaterial(name + "/" + name);
	animator->ReadMesh(name + "/" + name);
	animator->ReadClip(name + "/Run");

	//animator->PlayClip(0);	
}

ModelScene::~ModelScene()
{	
	//delete kaya;

	//delete model;
	//delete modelRender;

	delete animator;
}

void ModelScene::Update()
{
	//kaya->Update();
	//modelRender->Update();
	animator->Update();
}

void ModelScene::PreRender()
{
}

void ModelScene::Render()
{
	//kaya->Render();
	//modelRender->Render();
	animator->Render();
}

void ModelScene::PostRender()
{
}

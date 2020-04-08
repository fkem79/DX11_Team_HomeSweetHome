#include "Framework.h"
#include "DoorWall.h"

DoorWall::DoorWall()
{
	model = new ModelAnimator(L"ModelAnimationInstancing");
	transform = model->AddTransform();
	ReadData();
	//model->SetEndEvent(ATTACK, bind(&Kaya::SetIdle, this));

	model->PlayClip(0, state);
	

	transform->scale = Vector3(0.05f, 0.05f, 0.05f);
}

DoorWall::~DoorWall()
{
}

void DoorWall::Update()
{
	model->Update();
}

void DoorWall::Render()
{
	model->Render();
}

void DoorWall::SetAnimation(AnimState value)
{
	if (state != value)
	{
		state = value;
		model->PlayClip(0, value);
	}
}

void DoorWall::SetIdle()
{
}

void DoorWall::ReadData()
{
	string name = "doorWall01";
	
	ModelReader* reader = new ModelReader();
	reader->ReadFile("ModelData/Models/" + name + ".fbx");
	reader->ExportMaterial(name + "/" + name);
	reader->ExportMesh(name + "/" + name);
	delete reader;
	
	reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/" + name + "/doorWall01.fbx");
	reader->ExportAnimClip(0, name + "/doorWall01");
	delete reader;
	/*

	reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/" + name + "/Idle.fbx");
	reader->ExportAnimClip(0, name + "/Idle");
	delete reader;

	reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/" + name + "/Running.fbx");
	reader->ExportAnimClip(0, name + "/Running");
	delete reader;

	reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/" + name + "/Attack.fbx");
	reader->ExportAnimClip(0, name + "/Attack");
	delete reader;
	*/

	string path = name + "/" + name;
	model->ReadMaterial(path);
	model->ReadMesh(path);
	model->ReadClip(name + "Idle");

}

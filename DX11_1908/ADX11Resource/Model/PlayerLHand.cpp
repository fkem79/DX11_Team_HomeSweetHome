#include "Framework.h"
#include "PlayerLHand.h"

PlayerLHand::PlayerLHand()
	: moveSpeed(10.0f), rotSpeed(2.0f), state(HOLDING), accelation(10.0f),
	deceleration(3.0f), velocity(0, 0, 0)
{
	model = new ModelAnimator(L"ModelAnimationInstancing");
	//model = new ModelAnimator(L"ModelInstancing");
	transform = model->AddTransform();
	ReadData();
	model->SetEndEvent(CHECK, bind(&PlayerLHand::SetIdle, this));

	model->PlayClip(0, state);

//	transform->scale = Vector3(0.05f, 0.05f, 0.05f);
	transform->scale = Vector3(0.85f, 0.85f, 0.85f);

	model->GetModel()->SetSpecular(Float4(0.0f, 0.0f, 0.0f, 1.0f));
}
PlayerLHand::~PlayerLHand()
{
	delete model;
}

void PlayerLHand::Update()
{
	/*if (KEYPRESS(VK_UP))
		velocity -= transform->GetForward() * accelation * DELTA;
	if (KEYPRESS(VK_DOWN))
		velocity += transform->GetForward() * accelation * DELTA;

	if (KEYPRESS(VK_RIGHT))
		transform->rotation += transform->GetUp() * rotSpeed * DELTA;
	if (KEYPRESS(VK_LEFT))
		transform->rotation -= transform->GetUp() * rotSpeed * DELTA;

	if (KEYDOWN(VK_SPACE))
		SetAnimation(ATTACK);*/

	//if (KEYUP('G'))
	//	SetAnimation(CHECK);

	model->Update();
	transform->UpdateWorld();
}

void PlayerLHand::Render()
{
	model->Render();
}

void PlayerLHand::SetAnimation(AnimState value)
{
	if (state != value)
	{
		state = value;
		model->PlayClip(0, value);
	}
}

void PlayerLHand::SetIdle()
{
	SetAnimation(HOLDING);
}

void PlayerLHand::ReadData()
{
	string name = "PlayerLHand";

	ModelReader* reader = new ModelReader();
	reader->ReadFile("ModelData/Models/" + name + ".fbx");
	reader->ExportMaterial(name + "/" + name);
	reader->ExportMesh(name + "/" + name);
	delete reader;

	reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/" + name + "/Holding.fbx");
	reader->ExportAnimClip(0, name + "/Holding");
	delete reader;

	reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/" + name + "/Check.fbx");
	reader->ExportAnimClip(0, name + "/Check");
	delete reader;

	string path = name + "/" + name;
	model->ReadMaterial(path);
	model->ReadMesh(path);
	model->ReadClip(name + "/Holding");
	model->ReadClip(name + "/Check");
	
	//model->ReadClip(name + "/Idle");
	//model->ReadClip(name + "/Idle");

}

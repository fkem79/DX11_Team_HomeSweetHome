#include "Framework.h"

Kaya::Kaya()
	: moveSpeed(30.0f), rotSpeed(2.0f), state(IDLE), accelation(10.0f),
	deceleration(3.0f), velocity(0, 0, 0)
{
	model = new ModelAnimator(L"ModelAnimationInstancing");
	transform = model->AddTransform();
	ReadData();	
	model->SetEndEvent(ATTACK, bind(&Kaya::SetIdle, this));

	model->PlayClip(0, state);

	transform->scale = Vector3(0.05f, 0.05f, 0.05f);
}

Kaya::~Kaya()
{
	delete model;
}

void Kaya::Update()
{	
	Input();
	Move();

	model->Update();	
}

void Kaya::Render()
{
	model->Render();
}

void Kaya::Input()
{
	if (KEYPRESS(VK_UP))
		velocity -= transform->GetForward() * accelation * DELTA;
	if (KEYPRESS(VK_DOWN))
		velocity += transform->GetForward() * accelation * DELTA;

	if (KEYPRESS(VK_RIGHT))
		transform->rotation += transform->GetUp() * rotSpeed * DELTA;
	if (KEYPRESS(VK_LEFT))
		transform->rotation -= transform->GetUp() * rotSpeed * DELTA;

	if (KEYDOWN(VK_SPACE))
		SetAnimation(ATTACK);
}

void Kaya::Move()
{
	if (state == ATTACK)
		return;

	float magnitude = velocity.Length();

	if (magnitude > 1.0f)
		velocity.Normalize();

	if (magnitude > 0.1f)
	{
		transform->position += velocity * moveSpeed * DELTA;

		SetAnimation(RUN);

		velocity = XMVectorLerp(velocity.data, XMVectorZero(), deceleration * DELTA);
	}
	else
	{
		SetAnimation(IDLE);
	}	
}

void Kaya::SetAnimation(AnimState value)
{
	if (state != value)
	{
		state = value;
		model->PlayClip(0, value);
	}
}

void Kaya::SetIdle()
{
	SetAnimation(IDLE);
}

void Kaya::ReadData()
{
	string name = "Kaya";

	/*
	ModelReader* reader = new ModelReader();
	reader->ReadFile("ModelData/Models/" + name + ".fbx");
	reader->ExportMaterial(name + "/" + name);
	reader->ExportMesh(name + "/" + name);
	delete reader;

	reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/" + name + "/Capoeira.fbx");
	reader->ExportAnimClip(0, name + "/Capoeira");
	delete reader;
	

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
	model->ReadClip(name + "/Capoeira");
	model->ReadClip(name + "/Idle");
	model->ReadClip(name + "/Running");
	model->ReadClip(name + "/Attack");
}

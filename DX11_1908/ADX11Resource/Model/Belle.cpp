#include "Framework.h"
#include "Belle.h"

Belle::Belle()
	: moveSpeed(10.0f), rotSpeed(2.0f), state(IDLE), accelation(10.0f),
	deceleration(3.0f), velocity(0, 0, 0)
{
	model = new ModelAnimator(L"ModelAnimationInstancing");
	//model = new ModelAnimator(L"ModelInstancing");
	transform = model->AddTransform();
	ReadData();
	model->SetEndEvent(ATTACK, bind(&Belle::SetIdle, this));

	model->PlayClip(0, state);

	transform->scale = Vector3(0.1f, 0.1f, 0.1f);

	model->GetModel()->SetSpecular(Float4(0.0f, 0.0f, 0.0f, 1.0f));
}

Belle::~Belle()
{
	delete model;
}

void Belle::Update()
{
	Input();
	Move();

	model->Update();
}
void Belle::Render()
{
	model->Render();
}

void Belle::Input()
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

	if (KEYDOWN(VK_CONTROL))
		SetAnimation(AIM);

	if (KEYDOWN(VK_NUMPAD0))
		SetAnimation(APPEAR);

	if (KEYDOWN(VK_NUMPAD1))
		SetAnimation(APPEARWALL);

	if (KEYDOWN(VK_NUMPAD2))
		SetAnimation(CONFUSE);

	if (KEYDOWN(VK_NUMPAD3))
		SetAnimation(HOLD);

	if (KEYDOWN(VK_NUMPAD4))
		SetAnimation(PURSUITWALK);

	if (KEYDOWN(VK_NUMPAD5))
		SetAnimation(ROAR);

	if (KEYDOWN(VK_NUMPAD6))
		SetAnimation(STARTROAR);


	//if (KEYDOWN(VK_F3))
		//SetAnimation(ROAR);

	/*
	if (KEYDOWN(VK_F3))
		SetAnimation(TurnLeft);
	if (KEYDOWN(VK_F4))
		SetAnimation(TurnRight);*/
}

void Belle::Move()
{
	if (state == AIM || state == APPEAR || state == APPEARWALL || state == ATTACK || state == CONFUSE || state == HOLD ||
		state == ROAR || state == STARTROAR)
		return;

	float magnitude = velocity.Length();

	if (magnitude > 1.0f)
		velocity.Normalize();

	if (magnitude > 0.1f)
	{
		transform->position += velocity * moveSpeed * DELTA;

		SetAnimation(WALK);

		velocity = XMVectorLerp(velocity.data, XMVectorZero(), deceleration * DELTA);
	}
	else
		SetAnimation(IDLE);

}

void Belle::SetAnimation(AnimState value)
{
	if (state != value)
	{
		state = value;
		model->PlayClip(0, value);
	}
}

void Belle::SetIdle()
{
	SetAnimation(IDLE);
}

void Belle::ReadData()
{
	string name = "Belle";

	ModelReader* reader = new ModelReader();
	reader->ReadFile("ModelData/Models/" + name + ".fbx");
	reader->ExportMaterial(name + "/" + name);
	reader->ExportMesh(name + "/" + name);
	delete reader;

	reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/" + name + "/Idle.fbx");
	reader->ExportAnimClip(0, name + "/Idle");
	delete reader;

	reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/" + name + "/Aim.fbx");
	reader->ExportAnimClip(0, name + "/Aim");
	delete reader;

	reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/" + name + "/Appear.fbx");
	reader->ExportAnimClip(0, name + "/Appear");
	delete reader;

	reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/" + name + "/AppearWall.fbx");
	reader->ExportAnimClip(0, name + "/AppearWall");
	delete reader;

	reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/" + name + "/Attack.fbx");
	reader->ExportAnimClip(0, name + "/Attack");
	delete reader;

	reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/" + name + "/Confuse.fbx");
	reader->ExportAnimClip(0, name + "/Confuse");
	delete reader;

	reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/" + name + "/Hold.fbx");
	reader->ExportAnimClip(0, name + "/Hold");
	delete reader;

	reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/" + name + "/PursuitWalk.fbx");
	reader->ExportAnimClip(0, name + "/PursuitWalk");
	delete reader;

	reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/" + name + "/Roar.fbx");
	reader->ExportAnimClip(0, name + "/Roar");
	delete reader;

	reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/" + name + "/Run.fbx");
	reader->ExportAnimClip(0, name + "/Run");
	delete reader;

	reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/" + name + "/StartRoar.fbx");
	reader->ExportAnimClip(0, name + "/StartRoar");
	delete reader;

	reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/" + name + "/Walk.fbx");
	reader->ExportAnimClip(0, name + "/Walk");
	delete reader;

	string path = name + "/" + name;
	model->ReadMaterial(path);
	model->ReadMesh(path);
	model->ReadClip(name + "/Idle");
	model->ReadClip(name + "/Aim");
	model->ReadClip(name + "/Appear");
	model->ReadClip(name + "/AppearWall");
	model->ReadClip(name + "/Attack");
	model->ReadClip(name + "/Confuse");
	model->ReadClip(name + "/Hold");
	model->ReadClip(name + "/PursuitWalk");
	model->ReadClip(name + "/Roar");
	model->ReadClip(name + "/Run");
	model->ReadClip(name + "/StartRoar");
	model->ReadClip(name + "/Walk");
}
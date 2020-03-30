#include "Framework.h"
#include "Belle.h"

Belle::Belle()
	: moveSpeed(30.0f), rotSpeed(2.0f), state(IDLE), accelation(10.0f),
	deceleration(3.0f), velocity(0, 0, 0)
{
	model = new ModelAnimator(L"ModelAnimationInstancing");
	//model = new ModelAnimator(L"ModelInstancing");
	transform = model->AddTransform();
	ReadData();
	model->SetEndEvent(ATTACK, bind(&Belle::SetIdle, this));

	model->PlayClip(0, state);

	transform->scale = Vector3(0.05f, 0.05f, 0.05f);
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
}

void Belle::Move()
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
	reader->ReadFile("ModelData/Animations/" + name + "/Walk.fbx");
	reader->ExportAnimClip(0, name + "/Walk");
	delete reader;

	reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/" + name + "/Attack.fbx");
	reader->ExportAnimClip(0, name + "/Attack");
	delete reader;

	string path = name + "/" + name;
	model->ReadMaterial(path);
	model->ReadMesh(path);
	model->ReadClip(name + "/Idle");
	model->ReadClip(name + "/Walk");
	model->ReadClip(name + "/Attack");
}

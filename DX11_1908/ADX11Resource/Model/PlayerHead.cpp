#include "Framework.h"
#include "PlayerHead.h"

PlayerHead::PlayerHead()
	: moveSpeed(10.0f), dashSpeed(25.0f), crouchSpeed(5.0f), rotSpeed(1.5f), mouseRotSpeed(3.0f), dashMouseRotSpeed(4.5f), state(IDLE), accelation(10.0f),
	deceleration(3.0f), velocity(0, 0, 0), oldMousePos(0, 0, 0), crouchOn(false), crouchValue(3.0f)
{
	model = new ModelAnimator(L"ModelAnimationInstancing");
	transform = model->AddTransform();
	ReadData();
	model->SetEndEvent(ATTACK, bind(&PlayerHead::SetIdle, this));

	model->PlayClip(0, state);

	transform->scale = Vector3(0.05f, 0.05f, 0.05f);

	model->GetModel()->SetSpecular(Float4(0.0f, 0.0f, 0.0f, 1.0f));
}

PlayerHead::~PlayerHead()
{
	delete model;
}

void PlayerHead::Update()
{
	//MouseControl();
	Input();
	Move();
	Crouch();

	model->Update();
}

void PlayerHead::Render()
{
	model->Render();
}

void PlayerHead::Input()
{
	if (KEYPRESS('W'))
		velocity -= transform->GetForward() * accelation * DELTA;
	if (KEYPRESS('S'))
		velocity += transform->GetForward() * accelation * DELTA;

	if (KEYPRESS('D'))
		velocity -= transform->GetRight() * accelation * DELTA;
	if (KEYPRESS('A'))
		velocity += transform->GetRight() * accelation * DELTA;

	if (KEYPRESS(VK_RBUTTON))
	{
		if (KEYPRESS('Q'))
			transform->rotation.SetX(transform->rotation.GetX()+0.1f);

	}

	if (KEYDOWN(VK_SPACE))
		SetAnimation(ATTACK);
}

void PlayerHead::Move()
{
	if (state == ATTACK)
		return;

	float magnitude = velocity.Length();

	if (magnitude > 1.0f)
		velocity.Normalize();

	if (magnitude > 0.1f)
	{
		
		if(KEYPRESS('C'))
			transform->position += velocity * crouchSpeed * DELTA;
		else if (KEYPRESS(VK_SHIFT))
			transform->position += velocity * dashSpeed * DELTA;
		else
			transform->position += velocity * moveSpeed * DELTA;

		SetAnimation(RUN);

		velocity = XMVectorLerp(velocity.data, XMVectorZero(), deceleration * DELTA);
	}
	else
		SetAnimation(IDLE);
}

void PlayerHead::Crouch()
{
	if (KEYDOWN('C'))
		crouchOn = true;

	if (crouchOn)
	{
		crouchOn = false;
		transform->position.SetY(transform->position.GetY() - crouchValue);
	}
	
	if (KEYUP('C'))
	{
		crouchOn = false;
		transform->position.SetY(transform->position.GetY() + crouchValue);
	}
}

void PlayerHead::SetAnimation(AnimState value)
{
	if (state != value)
	{
		state = value;
		model->PlayClip(0, value);
	}
}

void PlayerHead::SetIdle()
{
	SetAnimation(IDLE);
}

void PlayerHead::ReadData()
{
	string name = "PlayerHead";

	ModelReader* reader = new ModelReader();
	reader->ReadFile("ModelData/Models/" + name + ".fbx");
	reader->ExportMaterial(name + "/" + name);
	reader->ExportMesh(name + "/" + name);
	delete reader;

	reader = new ModelReader();
	reader->ReadFile("ModelData/Animations/" + name + "/Idle.fbx");
	reader->ExportAnimClip(0, name + "/Idle");
	delete reader;

	string path = name + "/" + name;
	model->ReadMaterial(path);
	model->ReadMesh(path);
	model->ReadClip(name + "/Idle");
	model->ReadClip(name + "/Idle");
	model->ReadClip(name + "/Idle");
	
}

void PlayerHead::MouseControl()
{
	//Vector3 val = MOUSEPOS - oldMousePos;
	auto mouse_state = g_mouse->GetState();

	Vector3 val = Vector3(mouse_state.x, mouse_state.y, 0);

	if (val.GetX() > 0.0f)
	{
		if (KEYPRESS(VK_SHIFT))
			transform->rotation += transform->GetUp() * dashMouseRotSpeed * DELTA;
		else
			transform->rotation += transform->GetUp() * mouseRotSpeed * DELTA;
	}

	if (val.GetX() < 0.0f)
	{
		if (KEYPRESS(VK_SHIFT))
			transform->rotation -= transform->GetUp() * dashMouseRotSpeed * DELTA;
		else
			transform->rotation -= transform->GetUp() * mouseRotSpeed * DELTA;
	}
		
	oldMousePos = MOUSEPOS;
}

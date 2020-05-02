#include "Framework.h"
#include "FPV.h"

FPV::FPV()
	: moveSpeed(15.0f), rotSpeed(1.5f), constMaxRotX(0.9f), constMinRotX(-0.5f), constYPos(17.0f), mouseControlOn(false)
	, walkSpeed(15.0f), dashSpeed(25.0f), crouchSpeed(5.0f), constCrouchYPos(13.5f)
{
	oldPos2 = { 0,0,0 };
	camNum = 1;
}

FPV::~FPV()
{
}

void FPV::Update()
{
	if (KEYUP(VK_F1))
		mouseControlOn = !mouseControlOn;

	if (KEYPRESS(VK_SHIFT))
		moveSpeed = dashSpeed;
	if (KEYPRESS('C'))
		moveSpeed = crouchSpeed;

	if (KEYUP(VK_SHIFT) || KEYUP('C'))
		moveSpeed = walkSpeed;

	
	

	Move();
	if (!mouseControlOn)
		return;

	Rota(); //이름 겹쳐서 Rota 라고 했습니다 원래는 로테이션

	View();
}

void FPV::PostRender()
{
	Camera::PostRender();
	ImGui::Checkbox("mouseControlOn(checkBox or F1 Key)", &mouseControlOn);

	if (mouseControlOn)
		g_mouse->SetMode(Mouse::MODE_RELATIVE);
	else
		g_mouse->SetMode(Mouse::MODE_ABSOLUTE);
}

void FPV::Move()
{
	if (KEYPRESS('W'))
		position += forward * moveSpeed * DELTA;
	if (KEYPRESS('S'))
		position -= forward * moveSpeed * DELTA;
	if (KEYPRESS('A'))
		position -= right * moveSpeed * DELTA;
	if (KEYPRESS('D'))
		position += right * moveSpeed * DELTA;

	if (this->rotation.x >= constMaxRotX)
		this->rotation.x = constMaxRotX;

	if (this->rotation.x <= constMinRotX)
		this->rotation.x = constMinRotX;

	if (KEYPRESS('C'))
	{
		if (position.y > constCrouchYPos)
			position.y -= 10.0f * DELTA;
		else if (position.y <= constCrouchYPos)
			position.y = constCrouchYPos;
	}
	else
	{
		if (position.y < constYPos)
			position.y += 15.0f * DELTA;
		else if (position.y >= constYPos)
			position.y = constYPos;
	}
	
}

void FPV::Rota()
{
	Vector3 value = { g_mouse->GetState().x - oldPos.x, g_mouse->GetState().y - oldPos.y,0 };
	Vector3 val = MOUSEPOS - oldPos2;

	rotation.SetX(rotation.GetX() + val.GetY() * rotSpeed * DELTA);
	rotation.SetY(rotation.GetY() + value.GetX() * rotSpeed * DELTA);

	Rotation();

	oldPos = Vector3(g_mouse->GetState().x, g_mouse->GetState().y, 0);
	oldPos2 = MOUSEPOS;

	if (position.y >= constYPos)
		position.y = constYPos;

	if (position.y <= constCrouchYPos)
		position.y = constCrouchYPos;

}

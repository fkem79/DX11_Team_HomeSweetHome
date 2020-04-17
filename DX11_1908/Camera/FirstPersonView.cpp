#include "Framework.h"
#include "FirstPersonView.h"

FirstPersonView::FirstPersonView()	
	: distance(0.0f), height(9.0f), moveDamping(130.0f),rotDamping(50), rotY(0),
	zoomSpeed(0.1f), destPos(0, 0, 0), destRot(0), targetOffsetYSpeed(7.0f),
	mouseRotSpeed(5.0f), dashMouseRotSpeed(6.5f), mouseControlOn(false)
{
	targetOffset = { 0, 7, 8 };
}

FirstPersonView::~FirstPersonView()
{
}

void FirstPersonView::Update()
{
	if (target == nullptr)
		return;

	Vector3 tempPos = Vector3(0, 0, -distance);

	if (rotDamping > 0.0f)
	{
		if (target->rotation.GetY() != destRot)
			destRot = LERP(destRot, target->rotation.GetY(), rotDamping * DELTA);

		matRotation = XMMatrixRotationY(destRot + XM_PI);
	}
	else
	{
		//MouseControl();
		matRotation = XMMatrixRotationY(rotY);
	}

	destPos = XMVector3TransformCoord(tempPos.data, matRotation);
	destPos += target->position;
	destPos.SetY(destPos.GetY() + height);

	float moveDampingDelta = moveDamping * DELTA;
	position = XMVectorLerp(position.data, destPos.data, 1 < moveDampingDelta ? 1 : moveDampingDelta);

	Vector3 tempOffset = XMVector3TransformCoord(targetOffset.data, matRotation);
	matView = XMMatrixLookAtLH(position.data, (target->position + tempOffset).data, up.data);

	MouseControl();

	if (KEYUP(VK_F1))
		mouseControlOn = !mouseControlOn;
}

void FirstPersonView::PostRender()
{
	ImGui::SliderFloat3("TargetOffset", (float*)&targetOffset, -20.0f, 20.0f);	
	ImGui::Checkbox("mouseControlOn(checkBox or F1 Key)", &mouseControlOn);

	if(mouseControlOn)
		g_mouse->SetMode(Mouse::MODE_RELATIVE);
	else
		g_mouse->SetMode(Mouse::MODE_ABSOLUTE);

}

void FirstPersonView::MouseControl()
{
	if (!mouseControlOn)
		return;

	auto mouseState = g_mouse->GetState();
	Vector3 val = Vector3(mouseState.x, mouseState.y, 0);

	if (val.GetY() > 0.0f)
		targetOffset.SetY(targetOffset.GetY() - (targetOffset.GetY() * targetOffsetYSpeed * DELTA));

	if (val.GetY() < 0.0f)
		targetOffset.SetY(targetOffset.GetY() + (targetOffset.GetY() * targetOffsetYSpeed * DELTA));

	// offset의 아래로 3.8 ~ 위로 13.0 
	// 수치 조정 필요하면 조정 가능하나 아래로 너무 내리면 박스(머리)가 보임
	if (targetOffset.GetY() < 3.8f)
		targetOffset.SetY(3.8f);

	if (targetOffset.GetY() > 13.0f)
		targetOffset.SetY(13.0f);

	if (target == nullptr)
		return;

	if (val.GetX() > 0.0f)
	{
		if (KEYPRESS(VK_SHIFT))
			target->rotation += target->GetUp() * dashMouseRotSpeed * DELTA;
		else
			target->rotation += target->GetUp() * mouseRotSpeed * DELTA;
	}

	if (val.GetX() < 0.0f)
	{
		if (KEYPRESS(VK_SHIFT))
			target->rotation -= target->GetUp() * dashMouseRotSpeed * DELTA;
		else
			target->rotation -= target->GetUp() * mouseRotSpeed * DELTA;
	}
}
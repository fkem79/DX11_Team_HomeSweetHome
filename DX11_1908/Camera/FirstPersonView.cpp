#include "Framework.h"
#include "FirstPersonView.h"

FirstPersonView::FirstPersonView()	
	: distance(0.0f), height(9.0f), targetOffset(0, 7, 8), moveDamping(30),rotDamping(50), rotY(0),
	zoomSpeed(0.1f), destPos(0, 0, 0), destRot(0), target(nullptr)
{
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
		{
			destRot = LERP(destRot, target->rotation.GetY(), rotDamping * DELTA);
		}

		matRotation = XMMatrixRotationY(destRot + XM_PI);
	}
	else
	{
		MouseControl();
		matRotation = XMMatrixRotationY(rotY);
	}

	destPos = XMVector3TransformCoord(tempPos.data, matRotation);

	destPos += target->position;
	destPos.SetY(destPos.GetY() + height);

	position = XMVectorLerp(position.data, destPos.data, moveDamping * DELTA);

	Vector3 tempOffset = XMVector3TransformCoord(targetOffset.data, matRotation);

	Vector3 t = { 2.0f, 0.0f, 0.0f };
	matView = XMMatrixLookAtLH(position.data, (target->position + tempOffset).data, up.data);

	MouseControl();
}

void FirstPersonView::PostRender()
{
	ImGui::SliderFloat3("TargetOffset", (float*)&targetOffset, -20.0f, 20.0f);	
}

void FirstPersonView::MouseControl()
{
	if (KEYPRESS(VK_RBUTTON))
	{
		Vector3 val = MOUSEPOS - oldPos;

		rotY += val.GetX() * 0.001f;

		targetOffset = { targetOffset.GetX()+ val.GetX() *0.01f, targetOffset.GetY() - val.GetY() * 0.01f, 8 };

		if (targetOffset.GetX() > 5.0f)
			targetOffset.SetX(5.0f);

		if (targetOffset.GetX() < -5.0f)
			targetOffset.SetX(-5.0f);

		if (targetOffset.GetY() >10.0f)
			targetOffset.SetY(10.0f);

		if (targetOffset.GetY() < 0.0f)
			targetOffset.SetY(0.0f);
	}

	oldPos = MOUSEPOS;


	/*float mXPos, mYPos;

	mXPos = oldPos.GetX();
	mYPos = oldPos.GetY();

	if (mXPos > 20.0f)
		mXPos = 20.0f;

	if (mXPos < -20.0f)
		mXPos = -20.0f;

	if (mYPos > 20.0f)
		mYPos = 20.0f;

	if (mYPos < -20.0f)
		mYPos = -20.0f;

	
	//targetOffset = { MOUSEPOS.GetX(), MOUSEPOS.GetY(), 8 };
	targetOffset = { mXPos, 7, 8 };
	*/
	//distance -= Keyboard::Get()->GetWheel() * zoomSpeed;
	//height -= Keyboard::Get()->GetWheel() * zoomSpeed;

	
}

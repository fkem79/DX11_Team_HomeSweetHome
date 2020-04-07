#include "Framework.h"
#include "FirstPersonView.h"

FirstPersonView::FirstPersonView()	
	: distance(0.0f), height(9.0f), targetOffset(0, 7, 8), moveDamping(70),rotDamping(50), rotY(0),
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
	matView = XMMatrixLookAtLH(position.data, (target->position + tempOffset).data, up.data);
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
	}

	oldPos = MOUSEPOS;

}
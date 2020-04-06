#include "Framework.h"
#include "FollowCam.h"

FollowCam::FollowCam()
	: distance(0), height(0), targetOffset(0, 5, 5), moveDamping(5),
	rotDamping(50), rotY(0), zoomSpeed(0.1f), destPos(0, 0, 0), destRot(0),
	target(nullptr)
{
}

FollowCam::~FollowCam()
{
}

void FollowCam::Update()
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
	//destPos += positionOffset;

	position = XMVectorLerp(position.data, destPos.data, moveDamping * DELTA);	

	Vector3 tempOffset = XMVector3TransformCoord(targetOffset.data, matRotation);

	Vector3 t = { 2.0f, 0.0f, 0.0f };
	matView = XMMatrixLookAtLH(position.data, (target->position + tempOffset).data, up.data);
	//matView = XMMatrixLookAtLH(t.data, (target->position + tempOffset).data, up.data);
	

	//matview
}

void FollowCam::PostRender()
{
	Camera::PostRender();
	ImGui::SliderFloat("Distance", &distance, -100.0f, 100.0f);
	ImGui::SliderFloat("Height", &height, 1.0f, 100.0f);
	ImGui::SliderFloat("MoveDamping", &moveDamping, 0.0f, 30.0f);
	ImGui::SliderFloat("RotDamping", &rotDamping, 0.0f, 30.0f);
	//Float3 tempOffset = offset;
	ImGui::SliderFloat3("TargetOffset", (float*)&targetOffset, -20.0f, 20.0f);	
	//offset = tempOffset;
}

void FollowCam::MouseControl()
{
	if (KEYPRESS(VK_RBUTTON))
	{
		Vector3 val = MOUSEPOS - oldPos;

		rotY += val.GetX() * 0.001f;		
	}

	oldPos = MOUSEPOS;

	distance -= Keyboard::Get()->GetWheel() * zoomSpeed;
	height -= Keyboard::Get()->GetWheel() * zoomSpeed;

	/*if (distance < 1.0f)
		distance = 1.0f;

	if (height < 1.0f)
		height = 1.0f;*/
}

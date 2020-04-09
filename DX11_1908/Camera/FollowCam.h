#pragma once

class FollowCam : public Camera
{
private:
	float distance;
	float height;

	Vector3 destPos;
	float destRot;

	float moveDamping;
	float rotDamping;

	float rotY;
	float rotSpeed;

	float zoomSpeed;

public:
	FollowCam();
	~FollowCam();

	void Update() override;
	void PostRender() override;

	void MouseControl();

	void SetTarget(Transform* value) override { target = value; }
};
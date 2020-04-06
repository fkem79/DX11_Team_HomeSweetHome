#pragma once
class FirstPersonView : public Camera
{
private:
	float distance;
	float height;

	Vector3 targetOffset;

	Vector3 destPos;
	float destRot;

	float moveDamping;
	float rotDamping;

	float rotY;
	float rotSpeed;

	float zoomSpeed;

	Transform* target;

public:
	FirstPersonView();
	~FirstPersonView();

	void Update() override;
	void PostRender() override;

	void MouseControl();

	void SetTarget(Transform* value) override { target = value; }
};


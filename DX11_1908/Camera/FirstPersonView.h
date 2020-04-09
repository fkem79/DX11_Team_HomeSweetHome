#pragma once
class FirstPersonView : public Camera
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

	float targetOffsetYSpeed;

	float mouseRotSpeed;		// 마우스로 움직일 때 
	float dashMouseRotSpeed;	// 마우스로 움직이고 대쉬하고 있을 때

	bool mouseControlOn;

public:
	FirstPersonView();
	~FirstPersonView();

	void Update() override;
	void PostRender() override;

	void MouseControl();

	void SetTarget(Transform* value) override { target = value; }
};


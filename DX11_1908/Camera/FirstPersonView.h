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

	float mouseRotSpeed;		// ���콺�� ������ �� 
	float dashMouseRotSpeed;	// ���콺�� �����̰� �뽬�ϰ� ���� ��

	bool mouseControlOn;

public:
	FirstPersonView();
	~FirstPersonView();

	void Update() override;
	void PostRender() override;

	void MouseControl();

	void SetTarget(Transform* value) override { target = value; }
};


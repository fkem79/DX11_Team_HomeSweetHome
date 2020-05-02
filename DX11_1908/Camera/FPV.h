#pragma once
class FPV : public Camera
{
private:
	float moveSpeed;
	float walkSpeed;
	float dashSpeed;
	float crouchSpeed;

	float rotSpeed;

	float constYPos, constCrouchYPos;
	float constMaxRotX, constMinRotX;

	bool mouseControlOn;

	Vector3 oldPos2;

public:
	FPV();
	~FPV();

	void Update() override;
	void PostRender() override;

	void Move();
	void Rota();

	void SetTarget(Transform* value) override { target = value; }

};

#pragma once
class PlayerHead
{
	enum AnimState
	{
		IDLE,
		RUN,
		ATTACK,
		KILL
	}state;

	Transform* transform;
	ModelAnimator* model;

	float rotSpeed;				// 방향키로 움직일 때 
	float mouseRotSpeed;		// 마우스로 움직일 때 
	float dashMouseRotSpeed;	// 마우스로 움직이고 대쉬하고 있을 때

	float accelation;
	float deceleration;

	Vector3 velocity;
	Vector3 oldMousePos;

	float moveSpeed;
	float dashSpeed;
	float crouchSpeed;
	float crouchValue;

	bool crouchOn;
	

public:
	PlayerHead();
	~PlayerHead();

	void Update();
	void Render();

	Transform* GetTransform() { return transform; }

	Model* GetModel() { return model->GetModel(); }

private:
	void Input();
	void Move();
	void Crouch();

	void SetAnimation(AnimState value);
	void SetIdle();

	void ReadData();

	void MouseControl();
};

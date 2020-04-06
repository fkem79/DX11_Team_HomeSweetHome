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

	float rotSpeed;				// ����Ű�� ������ �� 
	float mouseRotSpeed;		// ���콺�� ������ �� 
	float dashMouseRotSpeed;	// ���콺�� �����̰� �뽬�ϰ� ���� ��

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

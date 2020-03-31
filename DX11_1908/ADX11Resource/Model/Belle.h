#pragma once
class Belle
{
private:
	enum AnimState
	{
		IDLE,
		RUN,
		ATTACK,
		TurnLeft,
		TurnRight
	}state;

	Transform* transform;
	ModelAnimator* model;

	float moveSpeed;
	float rotSpeed;

	float accelation;
	float deceleration;

	Vector3 velocity;

public:
	Belle();
	~Belle();

	void Update();
	void Render();

	Transform* GetTransform() { return transform; }

	Model* GetModel() { return model->GetModel(); }
private:
	void Input();
	void Move();

	void SetAnimation(AnimState value);
	void SetIdle();

	void ReadData();

};

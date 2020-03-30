#pragma once

class Kaya
{
private:
	enum AnimState
	{
		DANCE,
		IDLE,
		RUN,
		ATTACK
	}state;

	Transform* transform;
	ModelAnimator* model;

	float moveSpeed;
	float rotSpeed;

	float accelation;
	float deceleration;

	Vector3 velocity;

public:
	Kaya();
	~Kaya();

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
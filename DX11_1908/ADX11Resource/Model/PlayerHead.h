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

	float moveSpeed;
	float rotSpeed;

	float accelation;
	float deceleration;

	Vector3 velocity;

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

	void SetAnimation(AnimState value);
	void SetIdle();

	void ReadData();
};

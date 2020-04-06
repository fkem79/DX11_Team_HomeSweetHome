#pragma once
class PlayerLHand
{
private:

	enum AnimState
	{
		IDLE,
		HOLDING,
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
	PlayerLHand();
	~PlayerLHand();

	void Update();
	void Render();

	Transform* GetTransform() { return transform; }

	Model* GetModel() { return model->GetModel(); }
private:

	void SetAnimation(AnimState value);
	void SetIdle();

	void ReadData();

};

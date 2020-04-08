#pragma once
class DoorWall
{
private:
	enum AnimState
	{
		IDLE
	}state;

	Transform* transform;
	ModelAnimator* model;

	float moveSpeed;
	float rotSpeed;

	float accelation;
	float deceleration;

	Vector3 velocity;

public:
	DoorWall();
	~DoorWall();

	void Update();
	void Render();

	Transform* GetTransform() { return transform; }

	Model* GetModel() { return model->GetModel(); }

private:

	void SetAnimation(AnimState value);
	void SetIdle();

	void ReadData();
};

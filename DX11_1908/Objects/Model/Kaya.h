#pragma once

class AStar;

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

	Vector3 destPos;
	vector<Vector3> path;

	Terrain* terrain;
	AStar* aStar;
public:
	Kaya();
	~Kaya();

	void Update();
	void Render();

	Transform* GetTransform() { return transform; }

	Model* GetModel() { return model->GetModel(); }

	void SetTerrain(Terrain* value) { terrain = value; }
	void SetAStar(AStar* value) { aStar = value; }
private:
	void Input();
	void Move();
	void MovePath();
	void Rotate();

	void SetAnimation(AnimState value);
	void SetIdle();

	void ReadData();
};
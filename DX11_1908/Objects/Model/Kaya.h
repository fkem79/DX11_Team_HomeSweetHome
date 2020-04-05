#pragma once

<<<<<<< HEAD
=======
class AStar;

>>>>>>> sub/master
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

<<<<<<< HEAD
=======
	Vector3 destPos;
	vector<Vector3> path;

	Terrain* terrain;
	AStar* aStar;
>>>>>>> sub/master
public:
	Kaya();
	~Kaya();

	void Update();
	void Render();

	Transform* GetTransform() { return transform; }

	Model* GetModel() { return model->GetModel(); }

<<<<<<< HEAD
private:
	void Input();
	void Move();
=======
	void SetTerrain(Terrain* value) { terrain = value; }
	void SetAStar(AStar* value) { aStar = value; }
private:
	void Input();
	void Move();
	void MovePath();
	void Rotate();
>>>>>>> sub/master

	void SetAnimation(AnimState value);
	void SetIdle();

	void ReadData();
};
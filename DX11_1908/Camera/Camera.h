#pragma once

class Camera
{
public:
	Vector3 position;
	Vector3 rotation;

protected:
	Vector3 forward;
	Vector3 right;
	Vector3 up;

	Matrix matRotation;
	Matrix matView;

	Vector3 oldPos;
public:
	Camera();

	virtual void Update();
	virtual void Move();
	virtual void Rotation();
	virtual void View();

	virtual void PostRender();
	virtual void SetTarget(Transform* value) {}

	Ray GetRay();

	Matrix GetView() { return matView; }
};
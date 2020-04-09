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

	Transform* target;
	Vector3 targetOffset;

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
	Transform* GetTargetTransform() { return target; }
	Vector3 GetTargetOffset() { return targetOffset; }
};
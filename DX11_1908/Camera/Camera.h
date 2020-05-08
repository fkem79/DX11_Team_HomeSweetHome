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

	//추가
	Matrix invMatView;
	UINT camNum;
	
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
	Matrix* GetCamView() { return &matView; }
	Matrix* GetCamRotation() { return &matRotation; }
	Matrix* GetCamInvView() { return &invMatView; }

	Transform* GetTargetTransform() { return target; }
	Vector3 GetTargetOffset() { return targetOffset; }

	UINT GetCamNum() { return camNum; }

	//추가

	Vector3 GetForward() { return forward; }
};
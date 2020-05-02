#pragma once

class WorldBuffer;

class Transform
{
public:
	string tag;

	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
	
protected:
	Vector3 pivot;

	Matrix world;
	Matrix* parent;

	WorldBuffer* worldBuffer;

public:
	Transform(string tag = "Untagged");
	virtual ~Transform();

	void UpdateWorld();
	void UpdateWorldAddWorld(Matrix* world);

	void SetParent(Matrix* matrix) { parent = matrix; }

	Vector3 GetRight();
	Vector3 GetUp();
	Vector3 GetForward();

	Vector3 GetWorldPosition();

	void SetWorldBuffer(UINT slot = 1);

	//Matrix GetWorld() { return XMMatrixTranspose(world); }		
	Matrix GetWorld() { return world; }
	Matrix* GetWorldPointer() { return &world; }
};
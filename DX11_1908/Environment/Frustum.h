#pragma once

class Frustum
{
private:
	Vector4 planes[6];

	Matrix projection;
public:
	Frustum();
	~Frustum();

	void Update();

	bool ContainPoint(Vector3 position);
	bool ContainCube(Vector3 center, float radius);
};
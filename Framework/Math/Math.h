#pragma once


namespace GameMath
{
	bool IntersectTri(const Vector3& rayPos, const Vector3& rayDir,
		const Vector3& p0, const Vector3& p1, const Vector3& p2, float* distance);

	float Distance(const Vector3& v1, const Vector3& v2);
	float Saturate(const float& value);

	int Random(int min, int max);
	float Random(float min, float max);	
}
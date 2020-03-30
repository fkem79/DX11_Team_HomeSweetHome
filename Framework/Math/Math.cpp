#include "Framework.h"

bool GameMath::IntersectTri(const Vector3& rayPos, const Vector3& rayDir, const Vector3& p0, const Vector3& p1, const Vector3& p2, float* distance)
{
    Vector3 edge1, edge2, h, s, q;
    float a, f, u, v;
    edge1 = p1 - p0;
    edge2 = p2 - p0;
    h = rayDir.Cross(edge2);
    a = edge1.Dot(h);

    if (a > -FLT_EPSILON && a < FLT_EPSILON)
        return false;

    f = 1.0f / a;
    s = rayPos - p0;
    u = f * s.Dot(h);

    if (u < 0.0f || u > 1.0f)
        return false;

    q = s.Cross(edge1);
    v = f * rayDir.Dot(q);
    if (v < 0.0f || u + v > 1.0f)
        return false;

    float t = f * edge2.Dot(q);
    if (t > FLT_EPSILON)
    {
        *distance = t;
        return true;
    }

    return false;
}

float GameMath::Distance(const Vector3& v1, const Vector3& v2)
{
	return XMVectorGetX(XMVector3Length((v2 - v1).data));
}

float GameMath::Saturate(const float& value)
{
    return max(0.0f, min(1.0f, value));
}

int GameMath::Random(int min, int max)
{
	return rand() % (max - min) + min;
}

float GameMath::Random(float min, float max)
{
    float normal = rand() / (float)RAND_MAX;

	return min + (max - min) * normal;
}

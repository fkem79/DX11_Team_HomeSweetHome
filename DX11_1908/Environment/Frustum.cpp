#include "Framework.h"
#include "Frustum.h"

Frustum::Frustum()
{	
	projection = XMMatrixPerspectiveFovLH(XM_PIDIV4, 1.0f, 0.1f, 1000.0f);
}

Frustum::~Frustum()
{
}

void Frustum::Update()
{
	Matrix V, P;
	V = CAMERA->GetView();
	//P = Environment::Get()->GetProjection();
	P = projection;

	Float4x4 matrix;
	XMStoreFloat4x4(&matrix, V* P);

	//Near
	float a = (float)(matrix._14 + matrix._13);
	float b = (float)(matrix._24 + matrix._23);
	float c = (float)(matrix._34 + matrix._33);
	float d = (float)(matrix._44 + matrix._43);	
	planes[0] = XMVectorSet(a, b, c, d);

	//Far
	a = (float)(matrix._14 - matrix._13);
	b = (float)(matrix._24 - matrix._23);
	c = (float)(matrix._34 - matrix._33);
	d = (float)(matrix._44 - matrix._43);
	planes[1] = XMVectorSet(a, b, c, d);

	//Left
	a = (float)(matrix._14 + matrix._11);
	b = (float)(matrix._24 + matrix._21);
	c = (float)(matrix._34 + matrix._31);
	d = (float)(matrix._44 + matrix._41);
	planes[2] = XMVectorSet(a, b, c, d);

	//Right
	a = (float)(matrix._14 - matrix._11);
	b = (float)(matrix._24 - matrix._21);
	c = (float)(matrix._34 - matrix._31);
	d = (float)(matrix._44 - matrix._41);
	planes[3] = XMVectorSet(a, b, c, d);

	//Bottom
	a = (float)(matrix._14 + matrix._12);
	b = (float)(matrix._24 + matrix._22);
	c = (float)(matrix._34 + matrix._32);
	d = (float)(matrix._44 + matrix._42);
	planes[4] = XMVectorSet(a, b, c, d);

	//Top
	a = (float)(matrix._14 - matrix._12);
	b = (float)(matrix._24 - matrix._22);
	c = (float)(matrix._34 - matrix._32);
	d = (float)(matrix._44 - matrix._42);
	planes[5] = XMVectorSet(a, b, c, d);

	for (UINT i = 0; i < 6; i++)
		planes[i] = XMPlaneNormalize(planes[i]);
}

bool Frustum::ContainPoint(Vector3 position)
{
	for (UINT i = 0; i < 6; i++)
	{		
		Vector3 dot = XMPlaneDotCoord(planes[i], position.data);

		if (dot.GetX() < 0.0f)
			return false;
	}

	return true;
}

bool Frustum::ContainCube(Vector3 center, float radius)
{
	Vector3 edge;
	Vector3 dot;

	for (UINT i = 0; i < 6; i++)
	{
		//1
		edge.SetX(center.GetX() - radius);
		edge.SetY(center.GetY() - radius);
		edge.SetZ(center.GetZ() - radius);
		dot = XMPlaneDotCoord(planes[i], edge.data);
		if (dot.GetX() > 0.0f)
			continue;

		//2
		edge.SetX(center.GetX() + radius);
		edge.SetY(center.GetY() - radius);
		edge.SetZ(center.GetZ() - radius);
		dot = XMPlaneDotCoord(planes[i], edge.data);
		if (dot.GetX() > 0.0f)
			continue;

		//3
		edge.SetX(center.GetX() + radius);
		edge.SetY(center.GetY() + radius);
		edge.SetZ(center.GetZ() - radius);
		dot = XMPlaneDotCoord(planes[i], edge.data);
		if (dot.GetX() > 0.0f)
			continue;
		//4
		edge.SetX(center.GetX() - radius);
		edge.SetY(center.GetY() - radius);
		edge.SetZ(center.GetZ() + radius);
		dot = XMPlaneDotCoord(planes[i], edge.data);
		if (dot.GetX() > 0.0f)
			continue;

		//5
		edge.SetX(center.GetX() + radius);
		edge.SetY(center.GetY() - radius);
		edge.SetZ(center.GetZ() + radius);
		dot = XMPlaneDotCoord(planes[i], edge.data);
		if (dot.GetX() > 0.0f)
			continue;

		//6
		edge.SetX(center.GetX() - radius);
		edge.SetY(center.GetY() + radius);
		edge.SetZ(center.GetZ() + radius);
		dot = XMPlaneDotCoord(planes[i], edge.data);
		if (dot.GetX() > 0.0f)
			continue;

		//7
		edge.SetX(center.GetX() - radius);
		edge.SetY(center.GetY() + radius);
		edge.SetZ(center.GetZ() - radius);
		dot = XMPlaneDotCoord(planes[i], edge.data);
		if (dot.GetX() > 0.0f)
			continue;

		//8
		edge.SetX(center.GetX() + radius);
		edge.SetY(center.GetY() + radius);
		edge.SetZ(center.GetZ() + radius);
		dot = XMPlaneDotCoord(planes[i], edge.data);
		if (dot.GetX() > 0.0f)
			continue;

		return false;
	}

	return true;
}

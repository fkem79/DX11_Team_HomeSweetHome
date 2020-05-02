#include "Framework.h"

Transform::Transform(string tag)
	: tag(tag), position(0, 0, 0), rotation(0, 0, 0), scale(1, 1, 1),
	pivot(0, 0, 0), parent(nullptr)
{
	world = XMMatrixIdentity();
	worldBuffer = new WorldBuffer();
}

Transform::~Transform()
{
	delete worldBuffer;
}

void Transform::UpdateWorld()
{
	world = XMMatrixTransformation(pivot.data, XMQuaternionIdentity(), scale.data, pivot.data,
		XMQuaternionRotationRollPitchYaw(rotation.GetX(), rotation.GetY(), rotation.GetZ()),
		position.data);

	if (parent != nullptr)
		world *= *parent;

	worldBuffer->SetWorld(world);
}

void Transform::UpdateWorldAddWorld(Matrix* world)
{
	this->world = *world;
	if (parent != nullptr)
		*world *= *parent;

	worldBuffer->SetWorld(*world);
}

Vector3 Transform::GetRight()
{		
	return XMVector3Normalize(XMVector3TransformNormal(kRight, world));
}

Vector3 Transform::GetUp()
{
	return XMVector3Normalize(XMVector3TransformNormal(kUp, world));
}

Vector3 Transform::GetForward()
{
	return XMVector3Normalize(XMVector3TransformNormal(kForward, world));
}

Vector3 Transform::GetWorldPosition()
{
	return XMVector3Transform(XMVectorZero(), world);
}

void Transform::SetWorldBuffer(UINT slot)
{
	worldBuffer->SetVSBuffer(slot);
}


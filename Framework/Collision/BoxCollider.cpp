#include "Framework.h"

BoxCollider::BoxCollider(Float3 min, Float3 max)
	: minBox(min), maxBox(max)
{
	vertices.resize(8);

	vertices[0] = Vertex(min.x, min.y, min.z);
	vertices[1] = Vertex(min.x, max.y, min.z);
	vertices[2] = Vertex(max.x, max.y, min.z);
	vertices[3] = Vertex(max.x, min.y, min.z);

	vertices[4] = Vertex(min.x, min.y, max.z);
	vertices[5] = Vertex(min.x, max.y, max.z);
	vertices[6] = Vertex(max.x, max.y, max.z);
	vertices[7] = Vertex(max.x, min.y, max.z);

	indices = {
		0, 3, 1, 2, 5, 6, 4, 7,
		0, 4, 1, 5, 2, 6, 3, 7,
		0, 1, 3, 2, 7, 6, 4, 5
	};

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(Vertex), vertices.size());
	indexBuffer = new IndexBuffer(indices.data(), indices.size());
}

BoxCollider::~BoxCollider()
{
}

bool BoxCollider::IsCollision(Collider* collider)
{
	BoxCollider* coll = dynamic_cast<BoxCollider*>(collider);

	if (coll != nullptr)
		return IsBoxCollision(coll);
	
	return IsSphereCollision(static_cast<SphereCollider*>(collider));	
}

bool BoxCollider::IsCollision(IN Ray ray, OUT float* distance, OUT Vector3* contact)
{
	Vector3 minContact;
	float minDistance = FLT_MAX;

	for (UINT i = 0; i < 6; i++)
	{
		Vector3 p[4];

		p[0] = vertices[indices[i * 4 + 0]].position;
		p[1] = vertices[indices[i * 4 + 1]].position;
		p[2] = vertices[indices[i * 4 + 2]].position;
		p[3] = vertices[indices[i * 4 + 3]].position;

		p[0] = XMVector3TransformCoord(p[0].data, world);
		p[1] = XMVector3TransformCoord(p[1].data, world);
		p[2] = XMVector3TransformCoord(p[2].data, world);
		p[3] = XMVector3TransformCoord(p[3].data, world);

		float dist;
		if (IntersectTri(ray.position, ray.direction, p[0], p[1], p[3], &dist))
		{
			if (dist < minDistance)
			{
				minDistance = dist;
				minContact = ray.position + ray.direction * dist;
			}
		}

		if (IntersectTri(ray.position, ray.direction, p[2], p[1], p[3], &dist))
		{
			if (dist < minDistance)
			{
				minDistance = dist;
				minContact = ray.position + ray.direction * dist;
			}
		}
	}

	if (minDistance == FLT_MAX)
		return false;

	if (distance != nullptr)
		*distance = minDistance;

	if (contact != nullptr)
		*contact = minContact;

	return true;
}

Vector3 BoxCollider::GetMin()
{	
	return XMVector3TransformCoord(minBox.data, world);
}

Vector3 BoxCollider::GetMax()
{
	return XMVector3TransformCoord(maxBox.data, world);
}

bool BoxCollider::IsBoxCollision(BoxCollider* collider)
{
	Obb box1 = GetObb();
	Obb box2 = collider->GetObb();

	Vector3 position = box2.position - box1.position;

	if (SeperateAxis(position, box1.axisX, box1, box2)) return false;
	if (SeperateAxis(position, box1.axisY, box1, box2)) return false;
	if (SeperateAxis(position, box1.axisZ, box1, box2)) return false;

	if (SeperateAxis(position, box2.axisX, box1, box2)) return false;
	if (SeperateAxis(position, box2.axisY, box1, box2)) return false;
	if (SeperateAxis(position, box2.axisZ, box1, box2)) return false;

	if (box1.axisX == box2.axisX) return true;
	if (box1.axisX == box2.axisY) return true;
	if (box1.axisX == box2.axisZ) return true;

	if (box1.axisY == box2.axisX) return true;
	if (box1.axisY == box2.axisY) return true;
	if (box1.axisY == box2.axisZ) return true;

	if (box1.axisZ == box2.axisX) return true;
	if (box1.axisZ == box2.axisY) return true;
	if (box1.axisZ == box2.axisZ) return true;

	if (SeperateAxis(position, box1.axisX.Cross(box2.axisX), box1, box2)) return false;
	if (SeperateAxis(position, box1.axisX.Cross(box2.axisY), box1, box2)) return false;
	if (SeperateAxis(position, box1.axisX.Cross(box2.axisZ), box1, box2)) return false;

	if (SeperateAxis(position, box1.axisY.Cross(box2.axisX), box1, box2)) return false;
	if (SeperateAxis(position, box1.axisY.Cross(box2.axisY), box1, box2)) return false;
	if (SeperateAxis(position, box1.axisY.Cross(box2.axisZ), box1, box2)) return false;

	if (SeperateAxis(position, box1.axisZ.Cross(box2.axisX), box1, box2)) return false;
	if (SeperateAxis(position, box1.axisZ.Cross(box2.axisY), box1, box2)) return false;
	if (SeperateAxis(position, box1.axisZ.Cross(box2.axisZ), box1, box2)) return false;

	return true;
}


bool BoxCollider::IsSphereCollision(SphereCollider* collider)
{
	return collider->IsBoxCollision(this);
}

Obb BoxCollider::GetObb()
{
	Obb obb;

	obb.position = GetWorldPosition();

	obb.axisX = GetRight();
	obb.axisY = GetUp();
	obb.axisZ = GetForward();

	obb.halfSize = (maxBox - minBox) * 0.5f;
	obb.halfSize *= scale;

	return obb;
}

bool BoxCollider::SeperateAxis(Vector3 position, Vector3 direction, Obb box1, Obb box2)
{
	float d = abs(position.Dot(direction));

	float a = 0.0f;
	float b = 0.0f;

	a += abs((box1.axisX * box1.halfSize.GetX()).Dot(direction));
	a += abs((box1.axisY * box1.halfSize.GetY()).Dot(direction));
	a += abs((box1.axisZ * box1.halfSize.GetZ()).Dot(direction));

	b += abs((box2.axisX * box2.halfSize.GetX()).Dot(direction));
	b += abs((box2.axisY * box2.halfSize.GetY()).Dot(direction));
	b += abs((box2.axisZ * box2.halfSize.GetZ()).Dot(direction));

	return d > a + b;
}

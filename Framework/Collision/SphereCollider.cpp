#include "Framework.h"

SphereCollider::SphereCollider(float radius, UINT stackCount, UINT sliceCount)
	: radius(radius)
{
	float phiStep = XM_PI / stackCount;
	float thetaStep = XM_2PI / sliceCount;

	for (UINT i = 0; i <= stackCount; i++)
	{
		float phi = i * phiStep;

		for (UINT j = 0; j <= sliceCount; j++)
		{
			float theta = j * thetaStep;

			Vertex vertex;

			vertex.position.x = sin(phi) * cos(theta);
			vertex.position.y = cos(phi);
			vertex.position.z = sin(phi) * sin(theta);

			vertex.position = Vector3(vertex.position) * radius;

			vertices.push_back(vertex);
		}
	}

	for (UINT i = 0; i < stackCount; i++)
	{
		for (UINT j = 0; j < sliceCount; j++)
		{
			indices.push_back((sliceCount + 1) * i + j); //0			
			indices.push_back((sliceCount + 1) * i + j + 1); //1

			indices.push_back((sliceCount + 1) * i + j); //0			
			indices.push_back((sliceCount + 1) * (i + 1) + j); //2
		}
	}

	vertexBuffer = new VertexBuffer(vertices.data(), sizeof(Vertex), vertices.size());
	indexBuffer = new IndexBuffer(indices.data(), indices.size());
}

SphereCollider::~SphereCollider()
{
}

bool SphereCollider::IsCollision(Collider* collider)
{
	BoxCollider* coll = dynamic_cast<BoxCollider*>(collider);

	if (coll != nullptr)
		return IsBoxCollision(coll);

	return IsSphereCollision(static_cast<SphereCollider*>(collider));
}

bool SphereCollider::IsCollision(IN Ray ray, OUT float* distance, OUT Vector3* contact)
{
	Vector3 P = ray.position;
	Vector3 D = ray.direction;

	Vector3 C = GetWorldPosition();
	Vector3 A = P - C;

	float a = D.Dot(D);
	float b = 2.0f * D.Dot(A);
	float c = A.Dot(A) - pow(radius, 2);

	if (b * b >= 4 * a * c)
	{	
		if (distance != nullptr || contact != nullptr)
		{
			float t = (-b - sqrt(b * b - 4 * a * c)) / (2 * a);

			if (distance != nullptr)
				*distance = t;		

			if (contact != nullptr)
				*contact = P + D * t;			
		}		

		return true;
	}

	return false;
}

bool SphereCollider::IsBoxCollision(BoxCollider* collider)
{
	Obb obb = collider->GetObb();

	Vector3* axis = reinterpret_cast<Vector3*>(&obb.axisX);
	float size[3] = { obb.halfSize.GetX(), obb.halfSize.GetY(), obb.halfSize.GetZ() };
	
	Vector3 pos = obb.position;

	for (int i = 0; i < 3; i++)
	{
		float length = 0;
		length = axis[i].Dot((GetWorldPosition() - obb.position));

		float mult = (length < 0.0f) ? -1.0f : 1.0f;

		length = min(abs(length), size[i]);
		pos += axis[i] * length * mult;
	}

	return Distance(GetWorldPosition(), pos) <= radius;
}

bool SphereCollider::IsSphereCollision(SphereCollider* collider)
{
	float distance = Distance(GetWorldPosition(), collider->GetWorldPosition());

	return distance <= (radius + collider->radius);
}

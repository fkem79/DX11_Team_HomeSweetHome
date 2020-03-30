#include "Framework.h"

Camera::Camera()
	: position(0, 0, 0), rotation(0, 0, 0), 
	forward(0, 0, 1), right(1, 0, 0), up(0, 1, 0)
{
	oldPos = MOUSEPOS;

	matRotation = XMMatrixIdentity();
	matView = XMMatrixIdentity();

	Move();
	Rotation();
}

void Camera::Update()
{
}

void Camera::Move()
{	
}

void Camera::Rotation()
{
	Matrix rotX = XMMatrixRotationX(rotation.GetX());
	Matrix rotY = XMMatrixRotationY(rotation.GetY());

	matRotation = rotX * rotY;
		
	
	forward = XMVector3TransformNormal(kForward, matRotation);	
	up = XMVector3TransformNormal(kUp, matRotation);	
	right = XMVector3TransformNormal(kRight, matRotation);
}

void Camera::View()
{
	Vector3 focus = position + forward;
	matView = XMMatrixLookAtLH(position.data, focus.data, up.data);
}

void Camera::PostRender()
{
	ImGui::Text("Camera");
	ImGui::Text("Position(%f, %f, %f)", position.GetX(), position.GetY(), position.GetZ());
	ImGui::Text("Rotation(%f, %f)", rotation.GetX(), rotation.GetY());
}

Ray Camera::GetRay()
{
	Float2 screenSize = Float2(WIN_WIDTH, WIN_HEIGHT);

	Vector3 mousePos = MOUSEPOS;

	Float2 point;
	point.x = ((2 * mousePos.GetX()) / screenSize.x) - 1.0f;
	point.y = (((2 * mousePos.GetY()) / screenSize.y) - 1.0f) * -1.0f;

	Matrix projection = Environment::Get()->GetProjection();

	XMFLOAT4X4 temp;
	XMStoreFloat4x4(&temp, projection);

	point.x = point.x / temp._11;
	point.y = point.y / temp._22;

	Ray ray;

	ray.position = position;

	Matrix invView;
	invView = XMMatrixInverse(nullptr, matView);

	Vector3 pos = Vector3(point.x, point.y, 1);

	ray.direction = XMVector3TransformNormal(pos.data, invView);
	ray.direction.Normalize();

	return ray;
}

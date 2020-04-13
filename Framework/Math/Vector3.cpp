#include "Framework.h"
#include "Vector3.h"

Vector3::Vector3()
{
	data = XMVectorSet(0, 0, 0, 0);
}

Vector3::Vector3(Float3 value)
{
	data = XMLoadFloat3(&value);
}

Vector3::Vector3(float x, float y, float z)
{
	data = XMVectorSet(x, y, z, 0);
}

Vector3::Vector3(Vector4 value)
{
	data = value;
}

Vector3::operator Float3()
{
	Float3 result;
	XMStoreFloat3(&result, data);

	return result;
}

void Vector3::SetX(float value)
{
	data = XMVectorSetX(data, value);
}

void Vector3::SetY(float value)
{
	data = XMVectorSetY(data, value);
}

void Vector3::SetZ(float value)
{
	data = XMVectorSetZ(data, value);
}

float Vector3::GetX()
{	
	return XMVectorGetX(data);
}

float Vector3::GetY()
{
	return XMVectorGetY(data);
}

float Vector3::GetZ()
{
	return XMVectorGetZ(data);
}

Vector3 Vector3::operator+(const Vector3& value) const
{
	return Vector3(data + value.data);
}

Vector3 Vector3::operator-(const Vector3& value) const
{
	return Vector3(data - value.data);
}

Vector3 Vector3::operator*(const Vector3& value) const
{
	return Vector3(data * value.data);
}

Vector3 Vector3::operator/(const Vector3& value) const
{
	return Vector3(data / value.data);
}

void Vector3::operator+=(const Vector3& value)
{
	data += value.data;
}

void Vector3::operator-=(const Vector3& value)
{
	data -= value.data;
}

void Vector3::operator*=(const Vector3& value)
{
	data *= value.data;
}

void Vector3::operator/=(const Vector3& value)
{
	data /= value.data;
}

Vector3 Vector3::operator+(const float& value) const
{
	return data + XMVectorReplicate(value);
}

Vector3 Vector3::operator-(const float& value) const
{
	return data - XMVectorReplicate(value);
}

Vector3 Vector3::operator*(const float& value) const
{
	return data * XMVectorReplicate(value);
}

Vector3 Vector3::operator/(const float& value) const
{
	return data / XMVectorReplicate(value);
}

void Vector3::operator+=(const float& value)
{
	data += XMVectorReplicate(value);
}

void Vector3::operator-=(const float& value)
{
	data -= XMVectorReplicate(value);
}

void Vector3::operator*=(const float& value)
{
	data *= XMVectorReplicate(value);
}

void Vector3::operator/=(const float& value)
{
	data /= XMVectorReplicate(value);
}

bool Vector3::operator==(const Vector3& value)
{
	return XMVector3Equal(data, value.data);
}

Vector3 Vector3::Normal()
{
	return XMVector3Normalize(data);
}

void Vector3::Normalize()
{
	data = XMVector3Normalize(data);
}

float Vector3::Length()
{
	return XMVectorGetX(XMVector3Length(data));
}

Vector3 Vector3::Cross(const Vector3& value) const
{
	return XMVector3Cross(data, value.data);
}

float Vector3::Dot(const Vector3& value) const
{
	float result = 0.0f;
	XMStoreFloat(&result, XMVector3Dot(data, value.data));
	return result;
}

Vector3 Vector3::Cross(const Vector3& vec1, const Vector3& vec2)
{
	return XMVector3Cross(vec1.data, vec2.data);
}

float Vector3::Dot(const Vector3& vec1, const Vector3& vec2)
{
	float result = 0.0f;
	XMStoreFloat(&result, XMVector3Dot(vec1.data, vec2.data));
	return result;
}

Vector3 operator+(const float value1, const Vector3& value2)
{
	return value2.data + XMVectorReplicate(value1);
}

Vector3 operator-(const float value1, const Vector3& value2)
{
	return XMVectorReplicate(value1) - value2.data;
}

Vector3 operator*(const float value1, const Vector3& value2)
{
	return XMVectorReplicate(value1) * value2.data;
}

Vector3 operator/(const float value1, const Vector3& value2)
{
	return XMVectorReplicate(value1) / value2.data;
}

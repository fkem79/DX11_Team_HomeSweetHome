#pragma once

class Vector3
{
public:
	Vector4 data;

	Vector3();
	Vector3(Float3 value);
	Vector3(float x, float y, float z);
	Vector3(Vector4 value);

	operator Float3();

	void SetX(float value);
	void SetY(float value);
	void SetZ(float value);

	float GetX();
	float GetY();
	float GetZ();

	Vector3	 operator+(const Vector3& value) const;
	Vector3 operator-(const Vector3& value) const;
	Vector3 operator*(const Vector3& value) const;
	Vector3 operator/(const Vector3& value) const;

	void operator+=(const Vector3& value);
	void operator-=(const Vector3& value);
	void operator*=(const Vector3& value);
	void operator/=(const Vector3& value);

	Vector3 operator+(const float& value) const;
	Vector3 operator-(const float& value) const;
	Vector3 operator*(const float& value) const;
	Vector3 operator/(const float& value) const;

	friend Vector3 operator+(const float value1, const Vector3& value2);
	friend Vector3 operator-(const float value1, const Vector3& value2);
	friend Vector3 operator*(const float value1, const Vector3& value2);
	friend Vector3 operator/(const float value1, const Vector3& value2);

	void operator+=(const float& value);
	void operator-=(const float& value);
	void operator*=(const float& value);
	void operator/=(const float& value);

	bool operator==(const Vector3& value);

	Vector3 Normal();
	void Normalize();

	float Length();

	Vector3 Cross(const Vector3& value) const;
	float Dot(const Vector3& value) const;

	static Vector3 Cross(const Vector3& vec1, const Vector3& vec2);
	static float Dot(const Vector3& vec1, const Vector3& vec2);	
};
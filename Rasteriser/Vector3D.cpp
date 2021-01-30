#include "Vector3D.h"

Vector3D::Vector3D()
{

}

Vector3D::Vector3D(float x, float y, float z)
{
	_x = x;
	_y = y;
	_z = z;
}

//Copy Constructor
Vector3D::Vector3D(const Vector3D& v)
{
	_x = v.GetX();
	_y = v.GetY();
	_z = v.GetZ();
}

Vector3D::~Vector3D()
{

}

//Accessors and mutators
float Vector3D::GetX() const
{
	return _x;
}

void Vector3D::SetX(const float x)
{
	_x = x;
}

float Vector3D::GetY() const
{
	return _y;
}

void Vector3D::SetY(const float y)
{
	_y = y;
}

float Vector3D::GetZ() const
{
	return _z;
}

void Vector3D::SetZ(const float z)
{
	_z = z;
}


Vector3D& Vector3D::operator+ (const Vector3D &rhs)
{
	_x = _x + rhs.GetX();
	_y = _y + rhs.GetY();
	_z = _z + rhs.GetZ();
	return *this;
}
Vector3D& Vector3D::operator- (const Vector3D&rhs)
{
	_x = _x - rhs.GetX();
	_y = _y - rhs.GetY();
	_z = _z - rhs.GetZ();
	return *this;
}
Vector3D& Vector3D::operator/ (const float d)
{
	_x = _x / d;
	_y = _x / d;
	_z = _z / d;
	return *this;
}

Vector3D& Vector3D::operator= (const Vector3D &rhs)
{
	_x = rhs.GetX();
	_y = rhs.GetY();
	_z = rhs.GetZ();
	return *this;
}

// Returns The dot product of 2 vectors
float& Vector3D::DotProduct(const Vector3D& rhs) const
{
	float dotProductValue = _x * rhs.GetX() + _y * rhs.GetY() + _z * rhs.GetZ();
	return dotProductValue;
}

// Returns the cross product vector of two vectors
Vector3D Vector3D::CrossProduct(const Vector3D& rhs)const
{
	Vector3D crossProductVector
	{ _y * rhs.GetZ() - _z * rhs.GetY(),
		_z * rhs.GetX() - _x * rhs.GetZ(),
		_x * rhs.GetY() - _y * rhs.GetX()};
	return crossProductVector;
}
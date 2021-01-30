#pragma once
#include "Vector3D.h"
#include "stdafx.h"

class Vertex
{
public:
	Vertex();
	Vertex(float x, float y, float z, float w);

	//Copy constructor
	Vertex(const Vertex& v);

	//Destructor
	~Vertex();

	//Accessors and mutators
	float GetX() const;
	int GetIntX() const;
	void SetX(const float x);
	float GetY() const;
	int GetIntY() const;
	void SetY(const float y);
	float GetZ() const;
	void SetZ(const float z);
	float GetW() const;
	void SetW(const float w);
	COLORREF GetColor() const;
	void SetColor(COLORREF color);
	
	void Dehomogenize();

	//Assignment operator
	Vertex& operator= (const Vertex& rhs);

	bool operator== (const Vertex& rhs) const;

	const Vertex operator+ (const Vertex& rhs) const;

	const Vertex operator- (const Vertex& rhs) const;
	const Vector3D VectorFromVertex(const Vertex& rhs) const;
	Vector3D MinusToVector(const Vertex& other) const;
	Vector3D subVector(const Vertex& other) const;

private:
	float _x;
	float _y;
	float _z;
	float _w;

	COLORREF color;
	
};


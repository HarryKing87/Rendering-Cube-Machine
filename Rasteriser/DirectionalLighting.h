#pragma once
#include "Vector3D.h"

class DirectionalLight
{
public: 
	DirectionalLight();
	DirectionalLight(int red, int green, int blue, Vector3D movement);
	DirectionalLight(const DirectionalLight& light);


	// Get each color
	int GetRed() const;
	int GetGreen() const;
	int GetBlue() const;
	Vector3D GetMovement() const;

	DirectionalLight& operator= (const DirectionalLight& temp);

private:
	int Red_;
	int Green_;
	int Blue_;
	Vector3D movement_;
};

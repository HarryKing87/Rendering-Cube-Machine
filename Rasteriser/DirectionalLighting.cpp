#include "DirectionalLighting.h"

DirectionalLight::DirectionalLight()
{

}

DirectionalLight::DirectionalLight(int Red, int Green, int Blue, Vector3D movement)
{
	Red_ = Red;
	Green_ = Green;
	Blue_ = Blue;
	movement_ = movement;
}

DirectionalLight::DirectionalLight(const DirectionalLight& light)
{
	Red_ = light.GetRed();
	Green_ = light.GetGreen();
	Blue_ = light.GetBlue();
	movement_ = light.GetMovement();
}

// Getters

int DirectionalLight::GetRed() const {
	return Red_;
}

int DirectionalLight::GetGreen() const {
	return Green_;
}

int DirectionalLight::GetBlue() const {
	return Blue_;
}

Vector3D DirectionalLight::GetMovement() const {
	return movement_;
}

DirectionalLight& DirectionalLight::operator=(const DirectionalLight& temp)
{
	if (this != &temp)
	{
		Red_ = temp.GetRed();
		Green_ = temp.GetGreen();
		Blue_ = temp.GetBlue();
		movement_ = temp.GetMovement();
	}
	return *this;
}
#include "AmbientLighting.h"


AmbientLighting::AmbientLighting()
{

}

AmbientLighting::AmbientLighting(int red, int green, int blue)
{
	Red_ = red;
	Green_ = green;	
	Blue_ = blue;
}

AmbientLighting::AmbientLighting(const AmbientLighting& temp)
{
	Red_ = temp.GetRed();
	Green_ = temp.GetGreen();
	Blue_ = temp.GetBlue();
}

int AmbientLighting::GetRed() const
{
	return Red_;
}

int AmbientLighting::GetGreen() const
{
	return Green_;
}

int AmbientLighting::GetBlue() const
{
	return Blue_;
}

void AmbientLighting::SetRed(const int red)
{
	Red_ = red;
}

void AmbientLighting::SetGreen(const int green)
{
	Green_ = green;
}

void AmbientLighting::SetBlue(const int blue)
{
	Blue_ = blue;
}

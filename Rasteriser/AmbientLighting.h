#pragma once

class AmbientLighting
{

public:
	AmbientLighting();
	AmbientLighting(int red, int green, int blue);
	AmbientLighting(const AmbientLighting& temp);

	int GetRed() const;
	void SetRed(const int red);
	int GetGreen() const;
	void SetGreen(const int green);
	int GetBlue() const;
	void SetBlue(const int blue);

private:
	int Red_;
	int Green_;
	int Blue_;
};


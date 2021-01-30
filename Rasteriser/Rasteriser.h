#pragma once
#define _USE_MATH_DEFINES
#include <vector>
#include <time.h>
#include <math.h>
#include "Framework.h"
#include "Vertex.h"
#include "Matrix.h"
#include "Model.h"
#include "Polygon3d.h"
#include "MD2Loader.h"
#include "Camera.h"
#include "Vector3D.h"
#include "AmbientLighting.h"
#include "DirectionalLighting.h"


class Rasteriser : public Framework
{
public:
	string ModelPath();
	bool Initialise();
	void Update(Bitmap &bitmap);
	// Y Rotation Variables
	float rotationCubeX = 0.2;

	// X Rotation Variables
	float rotationCubeY = 0.2;

	// Z Rotation Variables
	float rotationCubeZ = 0.2;

	// X Translation Variables
	float translationCubeX = 0.2;
	float translateX = 0.2;
	float translateY = 0.2;
	float translateZ = 0.2;

	// Y Translation Variables
	float translationCubeY;

	// Z Translation Variables
	float translationCubeZ;

	float scale = 0.2;

	void Render(Bitmap &bitmap);
	void DrawWireFrame(Bitmap &bitmap);
	void DrawSolidFlat(Bitmap& bitmap);
	void DisplayString(Bitmap& bitmap, LPCTSTR text, int _x, int _y, int font);
	void GeneratePerspectiveMatrix(float d, float aspectRatio);
	void GenerateViewMatrix(float d, int width, int height);

private:
	Model _model;
	Camera _camera;
	Matrix _modelTransform;
	Matrix _perspectiveTransform;
	Matrix _screenTransform;
	string _modelpath;
	std::vector<DirectionalLight> lighting_;
	AmbientLighting lighting;
};


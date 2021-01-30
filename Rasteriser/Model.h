#pragma once
#include <vector>
#include "Polygon3D.h"
#include "Vertex.h"
#include "Matrix.h"
#include "Camera.h"
#include <algorithm>
#include "stdafx.h"
#include <math.h>
#include "DirectionalLighting.h"
#include "AmbientLighting.h"


class Model
{
public:
	Model();
	Model(float redAmbientLight, float greenAmbientLight, float blueAmbientLight, float redDirectionalLight, float greenDirectionalLight, float blueDirectionalLight);
	Model(const Model& m);
	~Model();

	//Accessors and mutators
	const std::vector<Polygon3D>& GetPolygons() const;
	const std::vector<Vertex>& GetVertices()const;
	const std::vector<Vertex>& GetTransform()const;

	// Getters and Setters for each color for each lighting 
	// feature

	float GetRedAmbient() const;
	float GetGreenAmbient() const;
	float GetBlueAmbient() const;

	void SetRedAmbient(const float lighting);
	void SetGreenAmbient(const float lighting);
	void SetBlueAmbient(const float lighting);

	float GetRedDirectional() const;
	float GetGreenDirectional() const;
	float GetBlueDirectional() const;

	void SetRedDirectional(const float lighting);
	void SetGreenDirectional(const float lighting);
	void SetBlueDirectional(const float lighting);

	// Setting every color calculated for light feature

	void SetColors(const float redAmbientLight, float greenAmbientLight, float blueAmbientLight, float redDirectionalLight, float greenDirectionalLight, float blueDirectionalLight);

	// Getting colors for the calculation
	COLORREF GetColor() const;
	
	size_t GetPolygonCount() const;
	size_t GetVertexCount() const;

	void AddVertex(float x, float y, float z);
	void AddPolygon(int i0, int i1, int i2);
	void firstVertexTransformation(const Matrix &transform);
	void ApplyTransformToTransformedVertices(const Matrix &transform);
	void Dehomogenize();
	void BackFaceCulling(Camera camera);
	void PolygonSorting(void);


	// Lighting
	void DirectionalLighting(std::vector<DirectionalLight> lighting);
	void AmbientLight(AmbientLighting lighting);
	Model& operator= (const Model& rhs);

private:
	std::vector<Polygon3D> _polygons;
	std::vector<Vertex> _vertices;
	std::vector<Vertex> _transform;

	float redAmbientLight_;
	float greenAmbientLight_;
	float blueAmbientLight_;

	float redDirectionalLight_;
	float greenDirectionalLight_;
	float blueDirectionalLight_;

	COLORREF color_;
};
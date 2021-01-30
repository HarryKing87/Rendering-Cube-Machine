#include "Model.h"
using namespace std;


Model::Model()
{
	_polygons.clear();
	_vertices.clear();
	_transform.clear();


	redAmbientLight_ = 0;
	greenAmbientLight_ = 0;
	blueAmbientLight_ = 0;

	redDirectionalLight_ = 0;
	greenDirectionalLight_ = 0;
	blueDirectionalLight_ = 0;
}

Model::Model(float redAmbientLight, float greenAmbientLight, float blueAmbientLight, float redDirectionalLight, float greenDirectionalLight, float blueDirectionalLight)
{
	_polygons.clear();
	_vertices.clear();
	_transform.clear();

redAmbientLight_ = redAmbientLight;
greenAmbientLight_ = greenAmbientLight;
blueAmbientLight_ = blueAmbientLight;

redDirectionalLight_ = redDirectionalLight;
greenDirectionalLight_ = greenDirectionalLight;
blueDirectionalLight_ = blueDirectionalLight;
}


//Copy Constructor
Model::Model(const Model& m)
{
	_polygons = m.GetPolygons();
	_vertices = m.GetVertices();
	_transform = m.GetTransform();

	redAmbientLight_ = m.GetRedAmbient();
	greenAmbientLight_ = m.GetGreenAmbient();
	blueAmbientLight_ = m.GetBlueAmbient();

	redDirectionalLight_ = m.GetRedDirectional();
	greenDirectionalLight_ = m.GetGreenDirectional();
	blueDirectionalLight_ = m.GetBlueDirectional();

	// finally getting the color
	color_ = m.GetColor();

}

Model::~Model()
{

}

//Accessors and mutators
const std::vector<Polygon3D>& Model::GetPolygons() const
{
	return _polygons;
}

const std::vector<Vertex>& Model::GetVertices() const
{
	return _vertices;
}

const std::vector<Vertex>& Model::GetTransform() const
{
	return _transform;
}


// Getting each value of every color used for each feature of light

// Ambient Light (Get)
float Model::GetRedAmbient() const {
	return redAmbientLight_;
}

float Model::GetGreenAmbient() const {
	return greenAmbientLight_;
}

float Model::GetBlueAmbient() const {
	return blueAmbientLight_;
}

// Ambient Light (Set)
void Model::SetRedAmbient(const float lighting)
{
	redAmbientLight_ = lighting;
}

void Model::SetGreenAmbient(const float lighting)
{
	greenAmbientLight_ = lighting;
}

void Model::SetBlueAmbient(const float lighting)
{
	blueAmbientLight_ = lighting;
}

// Directional Light (Get)
float Model::GetRedDirectional() const {
	return redDirectionalLight_;
}

float Model::GetGreenDirectional() const {
	return greenDirectionalLight_;
}

float Model::GetBlueDirectional() const {
	return blueDirectionalLight_;
}

// Directional Light (Set)
void Model::SetRedDirectional(const float lighting) {
	redDirectionalLight_ = lighting;
}

void Model::SetGreenDirectional(const float lighting) {
	greenDirectionalLight_ = lighting;
}

void Model::SetBlueDirectional(const float lighting) {
	blueDirectionalLight_ = lighting;
}

// Setting all colors that will be used to calculate each value of each lighting feature...
void Model::SetColors(const float redAmbientLight, float greenAmbientLight, float blueAmbientLight, float redDirectionalLight, float greenDirectionalLight, float blueDirectionalLight)
{
	redAmbientLight_ = redAmbientLight;
	greenAmbientLight_ = greenAmbientLight;
	blueAmbientLight_ = blueAmbientLight;

	redDirectionalLight_ = redDirectionalLight;
	greenDirectionalLight_ = greenDirectionalLight;
	blueDirectionalLight_ = blueDirectionalLight;
}


size_t Model::GetPolygonCount() const
{
	return _polygons.size();
}

size_t Model::GetVertexCount() const
{
	return _vertices.size();
}

COLORREF Model::GetColor() const {
	return color_;
}

void Model::AddVertex(float x, float y, float z)
{
	Vertex temp(x, y, z, 1);
	_vertices.push_back(temp);
}

void Model::AddPolygon(int i0, int i1, int i2)
{
	Polygon3D temp(i0, i1, i2);
	_polygons.push_back(temp);
	
}

void Model::firstVertexTransformation(const Matrix &transform)
{
	// Clearing the list before accessing it and inserting new values in it
	_transform.clear();

	// for every vertex we transform it and insert it into the list
	for (int i = 0; i < _vertices.size(); i++)
	{
		_transform.push_back(transform.operator*(_vertices[i]));

	}
}

void Model::ApplyTransformToTransformedVertices(const Matrix &Camera)
{
	// The same things continues for each vertex, while now we use the operator to insert it
	// with the camera
	for (int i = 0; i < _vertices.size(); i++)
	{
		_transform[i] = Camera.operator*(_transform[i]);
	}
}

void Model::Dehomogenize()
{

	// Finally dehomogenizing the vertices that are to be transformed
	for (int i = 0; i < _vertices.size(); i++)
	{
		_transform[i].Dehomogenize();
	}
}


Model& Model::operator=(const Model& rhs)
{
	//Only do the assignment if we are not assigning
	//to ourselves
	if (this != &rhs)
	{
		_polygons = rhs.GetPolygons();
		_vertices = rhs.GetVertices();
		_transform = rhs.GetTransform();
	}
	return *this;
}

void Model::BackFaceCulling( Camera camera)
{
	for (int i = 0; i < _polygons.size(); i++)
	{
		// For each polygon we get its index into a new variable name
		int index0 = _polygons[i].GetIndex(0);
		int index1 = _polygons[i].GetIndex(1);
		int index2 = _polygons[i].GetIndex(2);


		// Transforming each index and adding them to a Vertex variable
		Vertex ver0 = _transform[index0];
		Vertex ver1 = _transform[index1];
		Vertex ver2 = _transform[index2];

		// Calculating each Vertex with its previous A & B
		Vertex vecA{ ver0.operator-(ver1) };
		Vertex vecB{ ver0.operator-(ver2) };
		Vertex vecVecEye = ver0.operator-(camera.GetPos());

		// Getting each x,y,z value to their respected vectors
		Vector3D vectorA { vecA.GetX(),vecA.GetY(), vecA.GetZ() };
		Vector3D vectorB { vecB.GetX(),vecB.GetY(), vecB.GetZ() };
		// Also calculating eyeVector
		Vector3D eyeVector{ vecVecEye.GetX(),vecVecEye.GetY(),vecVecEye.GetZ() };

		// Normalization of vector calculated with the cross product
		_polygons[i].SetNormal(vectorA.CrossProduct(vectorB));

		// Calculation of each polygon using the eye Vector with the dot product
		float dotProduct{ _polygons[i].GetNormal().DotProduct(eyeVector) };


		// Finally setting the backface culling for each degree
		if (dotProduct < 0)
		{
			_polygons[i].SetCull(true);
		}
		else
		{
			_polygons[i].SetCull(false);

		}
	}
	

}


// Returns every side's polygon Z Average...
bool CalculateAverage(Polygon3D firstPolygon, Polygon3D secondPolygon)
{
	return firstPolygon.GetAverageZ() > secondPolygon.GetAverageZ();
}


// The main Polygon Sorting method
void Model::PolygonSorting(void)
{
	// For each polygon, we transform each index and each vertex
	// and then set their average Zs for each and every vertex transformed...
	for (int x = 0; x < _polygons.size(); x++)
	{
		int index0 = _polygons[x].GetIndex(0);
		int index1 = _polygons[x].GetIndex(1);
		int index2 = _polygons[x].GetIndex(2);

		Vertex ver0 = _transform[index0];
		Vertex ver1 = _transform[index1];
		Vertex ver2 = _transform[index2];

		_polygons[x].SetAverageZ((ver0.GetZ() + ver1.GetZ() + ver2.GetZ()) / 3);
	}
	// Finally, using the Sort method to begin and end sorting
	// each polygon based on the upper "Calculate Average" method
	// that returns every side's polygon Z Average
	std::sort(_polygons.begin(), _polygons.end(), CalculateAverage);
}


// Directional Lighting

void Model::DirectionalLighting(std::vector<DirectionalLight> lighting)
{
	float Red = 0;
	float Green = 0;
	float Blue = 0;

	float spotRed;
	float spotGreen;
	float spotBlue;

	for (int i = 0; i < _polygons.size(); i++)
	{
		Red = GetRValue(_polygons[i].GetColour());
		Green = GetGValue(_polygons[i].GetColour());
		Blue = GetBValue(_polygons[i].GetColour());

		for (int k = 0; k < lighting.size(); k++)
		{
			spotRed = float(lighting[k].GetRed());
			spotGreen = float(lighting[k].GetGreen());
			spotBlue = float(lighting[k].GetBlue());


			spotRed = spotRed * redDirectionalLight_;
			spotGreen = spotGreen * greenDirectionalLight_;
			spotBlue = spotBlue * blueDirectionalLight_;

			// Calculating normal of vectors
			float length = sqrt(lighting[k].GetMovement().GetX() * lighting[k].GetMovement().GetX()
				+ lighting[k].GetMovement().GetY() * lighting[k].GetMovement().GetY()
				+ lighting[k].GetMovement().GetZ() * lighting[k].GetMovement().GetZ());
			float normalX = lighting[k].GetMovement().GetX() / length;
			float normalY = lighting[k].GetMovement().GetY() / length;
			float normalZ = lighting[k].GetMovement().GetZ() / length;

			Vector3D normal(normalX, normalY, normalZ);

			// Getting the dot product of the precalculated normal vector and polygon
			float dotProduct = normal.DotProduct(_polygons[i].GetNormal());

			spotRed = spotRed * dotProduct;
			spotGreen = spotGreen * dotProduct;
			spotBlue = spotBlue * dotProduct;

			Red += spotRed;
			Green += spotGreen;
			Blue += spotBlue;

		}
		if (Red < 0 || Red > 255)
		{
			if (Red < 0)
			{
				Red = 0;
			}
			else
			{
				Red = 255;
			}
		}
		if (Green < 0 || Green > 255)
		{
			if (Green < 0)
			{
				Green = 0;
			}
			else
			{
				Green = 255;
			}
		}
		if (Blue < 0 || Blue > 255)
		{
			if (Blue < 0)
			{
				Blue = 0;
			}
			else
			{
				Blue = 255;
			}
		}

		// colorizing each section with the precalculated color
		COLORREF color_ = RGB(Red, Green, Blue);

		// Putting the colors inside the polygon
		_polygons[i].SetColour(color_);
	}
}

// Ambient Lighting
void Model::AmbientLight(AmbientLighting lighting)
{
	float Red;
	float Green;
	float Blue;

	float spotRed;
	float spotGreen;
	float spotBlue;


	for (int i = 0; i < (int)_polygons.size(); i++)
	{

		Red = 0;
		Green = 0;
		Blue = 0;

		spotRed = float(lighting.GetRed());
		spotGreen = float(lighting.GetGreen());
		spotBlue = float(lighting.GetBlue());

		spotRed = spotRed * redAmbientLight_;
		spotGreen = spotGreen * greenAmbientLight_;
		spotBlue = spotBlue * blueAmbientLight_;

		Red += spotRed;
		Green += spotGreen;
		Blue += spotBlue;

		if (Red < 0 || Red > 255)
		{
			if (Red < 0)
			{
				Red = 0;
			}
			else
			{
				Red = 255;
			}
		}
		if (Green < 0 || Green > 255)
		{
			if (Green < 0)
			{
				Green = 0;
			}
			else
			{
				Green = 255;
			}
		}
		if (Blue < 0 || Blue > 255)
		{
			if (Blue < 0)
			{
				Blue = 0;
			}
			else
			{
				Blue = 255;
			}
		}

		COLORREF color_ = RGB(Red, Green, Blue);

		_polygons[i].SetColour(color_);

		_transform[_polygons[i].GetIndex(0)].SetColor(color_);
		_transform[_polygons[i].GetIndex(1)].SetColor(color_);
		_transform[_polygons[i].GetIndex(2)].SetColor(color_);
	}
}






		
	


#include "Rasteriser.h"

Rasteriser app;

// The time limit for each feature to be displayed
const int timeline = 2000;


string Rasteriser::ModelPath()
{
	char buf[256];
	GetCurrentDirectoryA(256, buf);
	return std::string(buf) + '\\';
}

bool Rasteriser::Initialise()
{
	_modelpath="";
	_modelpath = ModelPath() + "\\cube.md2";

	if (!MD2Loader::LoadModel(_modelpath.c_str(), _model,
		&Model::AddPolygon,
		&Model::AddVertex))
	{
		return false;
	}

	// Setting each color for the model, regarding both the ambient and directional lights
	_model.SetColors((float)0.2, (float)0.2, (float)0.2, (float)0.5, (float)0.5, (float)0.5);

	// The camera's actual position
	// 1 is the W so it must be constantly 1 in order to be working good...
	Camera temp(0, 0, 0, Vertex(0, 0, -50, 1)); //Camera temp(0, 0, 0, Vertex(0, 7, -11, 1));
	_camera = temp;


	// Actual color of ambient light
	// Set it to be a shade of white
	lighting.SetRed(255);
	lighting.SetGreen(255);
	lighting.SetBlue(255);

	return true;
}

void Rasteriser::DrawWireFrame(Bitmap &bitmap)
{
	std::vector<Polygon3D> tempPolygon(_model.GetPolygons());
	int polygonSize = int(_model.GetPolygonCount());
	std::vector<Vertex> tempVertex(_model.GetTransform());

	HPEN hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	HGDIOBJ oldPen = SelectObject(bitmap.GetDC(), hPen);

	for (int i = 0; i < polygonSize; i++)
	{
		
		if (!tempPolygon[i].GetCull())
		{
			//Get the indices
			int tempIndex0 = tempPolygon[i].GetIndex(0);
			Vertex tempVertex0(tempVertex[tempIndex0]);
			int tempIndex1 = tempPolygon[i].GetIndex(1);
			Vertex tempVertex1(tempVertex[tempIndex1]);
			int tempIndex2 = tempPolygon[i].GetIndex(2);
			Vertex tempVertex2(tempVertex[tempIndex2]);
			
			//Draw the triangle
			MoveToEx(bitmap.GetDC(), tempVertex0.GetIntX(), tempVertex0.GetIntY(), NULL);
			LineTo(bitmap.GetDC(), tempVertex1.GetIntX(), tempVertex1.GetIntY());
			LineTo(bitmap.GetDC(), tempVertex2.GetIntX(), tempVertex2.GetIntY());
			LineTo(bitmap.GetDC(), tempVertex0.GetIntX(), tempVertex0.GetIntY());
		}
	}
	DeleteObject(hPen);
}

void Rasteriser::DrawSolidFlat(Bitmap& bitmap)
{
	std::vector<Polygon3D> tempPolygon(_model.GetPolygons());
	int polygonSize = int(_model.GetPolygonCount());
	std::vector<Vertex> tempVertex(_model.GetTransform());

	

	for (int i = 0; i < polygonSize; i++)
	{

		if (!tempPolygon[i].GetCull())
		{
			//Get the indices
			int tempIndex0 = tempPolygon[i].GetIndex(0);
			Vertex tempVertex0(tempVertex[tempIndex0]);
			int tempIndex1 = tempPolygon[i].GetIndex(1);
			Vertex tempVertex1(tempVertex[tempIndex1]);
			int tempIndex2 = tempPolygon[i].GetIndex(2);
			Vertex tempVertex2(tempVertex[tempIndex2]);

			HPEN hPen = CreatePen(PS_SOLID, 1, tempPolygon[i].GetColour());
			HGDIOBJ oldPen = SelectObject(bitmap.GetDC(), hPen);
			HBRUSH hBrush = CreateSolidBrush(tempPolygon[i].GetColour());
			HGDIOBJ oldBrush = SelectObject(bitmap.GetDC(), hBrush);


			POINT points[3] = { { double(tempVertex0.GetX()), double(tempVertex0.GetY()) }, {double(tempVertex1.GetX()), double(tempVertex1.GetY()) }, { double(tempVertex2.GetX()), double(tempVertex2.GetY()) }};
			Polygon(bitmap.GetDC(), points, 3);
			DeleteObject(hPen);            
			DeleteObject(hBrush);
		}
	}
}

void Rasteriser::DisplayString(Bitmap& bitmap, LPCTSTR text, int _x, int _y, int font)
{
	HDC hdc = bitmap.GetDC();     
	HFONT hFont, hOldFont;      
	hFont = hFont = CreateFont(font, 0, 0, 0, FW_DONTCARE, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, VARIABLE_PITCH, TEXT("Myfont"));      
	if (hOldFont = (HFONT)SelectObject(hdc, hFont)) 
	{
		SetTextColor(hdc, RGB(255, 255, 255));         
		SetBkMode(hdc, TRANSPARENT);          
		TextOut(hdc, _x, _y, text, lstrlen(text));          
		SelectObject(hdc, hOldFont); 
	}    
		DeleteObject(hFont);
}


void Rasteriser::GeneratePerspectiveMatrix(float d, float aspectRatio)
{
	Matrix matrixPerspectiveTransform{ 
		(d / aspectRatio), 0, 0, 0, 
		0, d, 0, 0, 
		0, 0, d, 0, 
		0, 0, 1, 0 
	};
	_perspectiveTransform = matrixPerspectiveTransform;
}

void Rasteriser::GenerateViewMatrix(float d, int width, int height)
{
	Matrix matrixscreenTransformation{ 
		(float)width / 2, 0, 0, 
		(float)width / 2, 0, (float)-height / 2, 0, 
		(float)height / 2, 0, 0, 
		d / 2, d / 2, 0, 0, 0, 1 
	};
	_screenTransform = matrixscreenTransformation ;
}


void Rasteriser::Update(Bitmap &bitmap)
{
	if (clock() <= 6 * timeline)
	{
		// X Rotation Animation is also animated in these two lines...
		rotationCubeX += (float)0.05;
		_modelTransform = Matrix::XRotationMatrix(rotationCubeX);
	}

	if (clock() <= 12 * timeline && clock() > 6 * timeline)
	{
		//Y Rotation Animation is applied here...
		rotationCubeY += (float)0.05;
		_modelTransform = Matrix::YRotationMatrix(rotationCubeY);
	}

	if (clock() <= 18 * timeline && clock() > 12 * timeline)
	{
		//Z Rotation Animation is applied here...
		rotationCubeZ += (float)0.05;
		_modelTransform = Matrix::ZRotationMatrix(rotationCubeZ);
	}

	if (clock() <= 20 * timeline && clock() > 18 * timeline)
	{
		// X Translation Animation for cube is translated below...
		scale += (float)0.0150;
		_modelTransform = Matrix::ScalingMatrix(scale, 1, 1) ;
	}

	if (clock() <= 22 * timeline && clock() > 20 * timeline)
	{
		scale -= (float)0.0150;
		_modelTransform = Matrix::ScalingMatrix(scale, 1, 1); //ADD SCALING MATRIX
	}

	if (clock() <= 24 * timeline && clock() > 22 * timeline) 
	{
		// X Translation Animation gets displayed below by zeroing the Y & Z...
		translationCubeY += (float)0.2;
		translateX += (float)0.2;
		translateY += (float)0.00;
		translateZ += (float)0.00;
		_modelTransform = Matrix::TranslationMatrix(translateX, translateY, translateZ);

	}
	if (clock() <= 26 * timeline && clock() > 24 * timeline)
	{
		// X Translation Animation gets displayed below by zeroing the Y & Z...
		translationCubeX += (float)0.2;
		translateX -= (float)0.2;
		translateY += (float)0.00;
		translateZ += (float)0.00;
		_modelTransform = Matrix::TranslationMatrix(translateX, translateY, translateZ);

	}

	if (clock() <= 28 * timeline && clock() > 26 * timeline)
	{
		// Y Translation Animation gets displayed below by zeroing the X & Z...
		translationCubeY += (float)0.2;
		translateX += (float)0.00;
		translateY += (float)0.2;
		translateZ += (float)0.00;
		_modelTransform = Matrix::TranslationMatrix(translateX, translateY, translateZ);

	}

	if (clock() <= 30 * timeline && clock() > 28 * timeline)
	{
		// Y Translation Animation gets displayed below by zeroing the X & Z...
		translationCubeY += (float)0.2;
		translateX += (float)0.00;
		translateY -= (float)0.2;
		translateZ += (float)0.00;
		_modelTransform = Matrix::TranslationMatrix(translateX, translateY, translateZ);

	}

	if (clock() <= 32 * timeline && clock() > 30 * timeline)
	{
		// Z Translation Animation gets displayed below by zeroing the X & Z...
		translationCubeY += (float)0.2;
		translateX += (float)0.00;
		translateY -= (float)0.00;
		translateZ += (float)0.2;
		_modelTransform = Matrix::TranslationMatrix(translateX, translateY, translateZ);

	}

	if (clock() <= 34 * timeline && clock() > 32 * timeline)
	{
		// Y Translation Animation gets displayed below by zeroing the X & Z...
		translationCubeY += (float)0.2;
		translateX += (float)0.00;
		translateY -= (float)0.00;
		translateZ -= (float)0.2;
		_modelTransform = Matrix::TranslationMatrix(translateX, translateY, translateZ);

	}

	if (clock() <= 36 * timeline && clock() > 34 * timeline)
	{
		// Rotation with translation?!?!!?!!?!??!??!?!...
		translateX += (float)0.00;
		translateY += (float)0.00;
		translateZ += (float)0.2;
		rotationCubeY += (float)0.05;
		_modelTransform = Matrix::YRotationMatrix(rotationCubeY) * Matrix::TranslationMatrix(translateX, translateY, translateZ);

	}

	if (clock() <= 38 * timeline && clock() > 36 * timeline)
	{
		// Rotation with translation?!?!!?!!?!??!??!?!...
		translateX += (float)0.00;
		translateY += (float)0.00;
		translateZ -= (float)0.2;
		rotationCubeY += (float)0.05;
		_modelTransform = Matrix::YRotationMatrix(rotationCubeY) * Matrix::TranslationMatrix(translateX, translateY, translateZ);
	}

	if (clock() <= 40 * timeline && clock() > 38 * timeline)
	{
		//Y Rotation Animation is applied here...
		rotationCubeY += (float)0.05;
		_modelTransform = Matrix::YRotationMatrix(rotationCubeY);
	}

	if (clock() <= 42 * timeline && clock() > 40 * timeline)
	{
		//Y Rotation Animation is applied here...
		rotationCubeY += (float)0.05;
		_modelTransform = Matrix::YRotationMatrix(rotationCubeY);
	}

	if (clock() <= 44 * timeline && clock() > 42 * timeline)
	{
		// Rotation with translation?!?!!?!!?!??!??!?!...
		translationCubeX += (float)0.2;
		translateX += (float)0.00;
		translateY += (float)0.2;
		translateZ += (float)0.00;
		rotationCubeY += (float)0.05;
		_modelTransform = Matrix::YRotationMatrix(rotationCubeY) * Matrix::TranslationMatrix(translateX, translateY, translateZ);

	}

	if (clock() <= 46 * timeline && clock() > 44 * timeline)
	{
		// Rotation with translation?!?!!?!!?!??!??!?!...
		translationCubeX += (float)0.2;
		translateX += (float)0.00;
		translateY -= (float)0.2;
		translateZ += (float)0.00;
		rotationCubeY += (float)0.05;
		_modelTransform = Matrix::YRotationMatrix(rotationCubeY) * Matrix::TranslationMatrix(translateX, translateY, translateZ);

	}


	float aspectRatio = float(float(bitmap.GetWidth()) / float(bitmap.GetHeight()));
	GeneratePerspectiveMatrix(1, aspectRatio);
	GenerateViewMatrix(1, bitmap.GetWidth(), bitmap.GetHeight());

}


void Rasteriser::Render(Bitmap &bitmap)
{
	



	
	
	bitmap.Clear(RGB(0, 0, 0));

	_model.firstVertexTransformation(_modelTransform);
	_model.ApplyTransformToTransformedVertices(_camera.GetCameraMatrix());
	// Applying Polygon Sorting to every polygon
	_model.PolygonSorting();
	_model.ApplyTransformToTransformedVertices(_perspectiveTransform);
	_model.Dehomogenize();
	_model.ApplyTransformToTransformedVertices(_screenTransform);

	if (clock() <= 6 * timeline)
	{
		DrawWireFrame(bitmap);
		DisplayString(bitmap, L"DrawWireframe + X-axis Rotation",10,55,35);
	}

	if (clock() <= 12 * timeline && clock() > 6 * timeline)
	{
		// Inserting backface culling method
		_model.BackFaceCulling(_camera);
		DrawWireFrame(bitmap);
		DisplayString(bitmap, L"DrawWireframe + Backface Culling + Y-axis Rotation", 10, 55, 35);
	}

	if (clock() <= 18 * timeline && clock() > 12 * timeline)
	{
		_model.BackFaceCulling(_camera);
		DrawSolidFlat(bitmap);
		DisplayString(bitmap, L"DrawSolidFlat + Z-axis Rotation", 10, 55, 35);
	}
	
	if (clock() <= 22 * timeline && clock() > 18 * timeline)
	{
		DrawWireFrame(bitmap);
		DisplayString(bitmap, L"DrawWireframe + Backface Culling + Scaling", 10, 55, 35);
	}

	if (clock() <= 26 * timeline && clock() > 22 * timeline)
	{
		_model.BackFaceCulling(_camera);
		DrawWireFrame(bitmap);
		DisplayString(bitmap, L"DrawWireframe + Backface Culling + Translation X-axis", 10, 55, 35);
	}

	if (clock() <= 30 * timeline && clock() > 26 * timeline)
	{
		_model.BackFaceCulling(_camera);
		DrawWireFrame(bitmap);
		DisplayString(bitmap, L"DrawWireframe + Backface Culling + Translation Y-axis", 10, 55, 35);
	}

	if (clock() <= 34 * timeline && clock() > 30 * timeline)
	{
		_model.BackFaceCulling(_camera);
		DrawWireFrame(bitmap);
		DisplayString(bitmap, L"DrawWireframe + Backface Culling + Translation Z-axis", 10, 55, 35);
	}

	if (clock() <= 38 * timeline && clock() > 34 * timeline)
	{
		_model.BackFaceCulling(_camera);
		DrawWireFrame(bitmap);
		DisplayString(bitmap, L"DrawWireframe + Backface Culling + Translation Z-axis and Rotation Y-axis", 10, 55, 35);
	}

	if (clock() <= 40 * timeline && clock() > 38 * timeline)
	{
		_model.BackFaceCulling(_camera);
		DrawSolidFlat(bitmap);
		DisplayString(bitmap, L"DrawSolidFlat + Backface Culling + Directional Light", 10, 55, 35);
		_model.DirectionalLighting(lighting_);
	}

	if (clock() <= 42 * timeline && clock() > 40 * timeline)
	{
		_model.BackFaceCulling(_camera);
		DrawSolidFlat(bitmap);
		_model.AmbientLight(lighting);
		DisplayString(bitmap, L"DrawSolidFlat + Backface Culling + Ambient Light", 10, 55, 35);
	}

	if (clock() <= 46 * timeline && clock() > 42 * timeline)
	{
		_model.BackFaceCulling(_camera);
		DrawWireFrame(bitmap);
		DisplayString(bitmap, L"DrawWireFrame + Backface Culling + Rotation & Translation in Y-axis", 10, 55, 35);
	}
}
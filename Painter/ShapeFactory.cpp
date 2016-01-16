#include "stdafx.h"
#include "ShapeFactory.h"
#include "Rectangle.h"
#include "Ellipse.h"
#include "line.h"
#include "Polygon.h"
#include "CompositShape.h"

using namespace std;

CShapeFactory::CShapeFactory()
{
}


CShapeFactory::~CShapeFactory()
{
}

std::shared_ptr<CShape> CShapeFactory::CreateShape(const TCHAR * shape_type)
{
	if (shape_type == _T("Rectangle"))
	{
		return shared_ptr<CShape>(new CRectangle);
	}
	else if (shape_type == _T("Ellipse"))
	{
		return  shared_ptr<CShape>(new CEllipse);
	}
	else if (shape_type == _T("Line"))
	{
		return shared_ptr<CShape>(new CLine);
	}
	else if (shape_type == _T("Polygon"))
	{
		return shared_ptr<CShape>(new CPolygon);
	}
	else if (shape_type == _T("Composite"))
	{
		return shared_ptr<CShape>(new CCompositShape);
	}

	return shared_ptr<CShape>();
}

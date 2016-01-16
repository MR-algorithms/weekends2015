#include "stdafx.h"
#include "ShapeFactory.h"
#include "Shape.h"

using namespace std;

CShapeFactory::CShapeFactory()
{
}


CShapeFactory::~CShapeFactory()
{
}

std::shared_ptr<CShape> CShapeFactory::CreateShape(const TCHAR * shape_type)
{
	auto iter = _shapes.find(shape_type);
	if (iter != _shapes.end())
	{
		return shared_ptr<CShape>(iter->second->Clone());
	}
	else
	{
		return shared_ptr<CShape>();
	}
}

bool CShapeFactory::InsertShape(const TCHAR * shape_type, std::shared_ptr<CShape> shape)
{
	auto iter = _shapes.find(shape_type);
	if (iter != _shapes.end())
	{
		return false;
	}

	_shapes.insert(make_pair (CString(shape_type), shape));
	
	return true;
}

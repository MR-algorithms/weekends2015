#pragma once

#include <memory>
class CShape;

class CShapeFactory
{
public:
	CShapeFactory();
	~CShapeFactory();

	std::shared_ptr<CShape> CreateShape(const TCHAR * shape_type);
};


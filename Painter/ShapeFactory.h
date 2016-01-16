#pragma once

#include <memory>
#include <map>

class CShape;

class CShapeFactory
{
public:
	CShapeFactory();
	~CShapeFactory();

	std::shared_ptr<CShape> CreateShape(const TCHAR * shape_type);
	bool InsertShape(const TCHAR * shape_type, std::shared_ptr<CShape> shape);
private:
	std::map < CString, std::shared_ptr<CShape>> _shapes;
};


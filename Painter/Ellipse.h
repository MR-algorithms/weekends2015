#pragma once
#include "Shape.h"
#include <gdiplus.h>
#include "..\Utilities\xml.h"

class CEllipse :
	public CShape
{
public:
	CEllipse();
	CEllipse(const Gdiplus::Point& point1, const Gdiplus::Point& point2);
	~CEllipse();

	virtual void Draw(Gdiplus::Graphics& graphics);
	virtual void Save(CArchive& ar);
	virtual void Save(Utilities::CXmlElement& element) override;

	virtual void Load(Utilities::CXmlElement& element) override;

	virtual int HitTest(const Gdiplus::Point& point) override;

private:
};


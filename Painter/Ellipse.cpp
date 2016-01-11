#include "stdafx.h"
#include "Ellipse.h"


CEllipse::CEllipse()
{
}

CEllipse::CEllipse(const Gdiplus::Point& point1, const Gdiplus::Point& point2) 
{
	SetRect(Gdiplus::Rect(point1.X, point1.Y, abs(point2.X - point1.X), abs(point2.Y - point1.Y)));
}

CEllipse::~CEllipse()
{
}

void CEllipse::Draw(Gdiplus::Graphics& graphics)
{
	Gdiplus::SolidBrush brush(GetFillColor());
	graphics.FillEllipse(&brush, _rect);

	Gdiplus::Pen pen(GetBorderColor());
	graphics.DrawEllipse(&pen, _rect);

	DrawBorder(graphics);
}

void CEllipse::Save(CArchive& ar)
{
	ar << int(ShapeEllipse);

	CShape::Save(ar);
}

void CEllipse::Save(Utilities::CXmlElement& element)
{
	element.SetAttrib(_T("Type"), _T("Ellipse"));
	__super::Save(element);
}

void CEllipse::Load(Utilities::CXmlElement& element)
{
	__super::Load(element);
}

int CEllipse::HitTest(const Gdiplus::Point& point)
{
	double a = _rect.Width / 2;
	double b = _rect.Height / 2;
	double m = _rect.GetLeft() + a;
	double n = _rect.GetTop() + b;

	double ellipse_value = (((point.X - m) / a) * ((point.X - m) / a) + ((point.Y - n) / b) * ((point.Y - n) / b));

	if (ellipse_value > 1 &&!_selected)
	{
		return HandleNone;
	}

		return CShape::HitTest(point);
}



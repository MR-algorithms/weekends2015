#include "stdafx.h"
#include "Line.h"

using namespace Gdiplus;

CLine::CLine()
{
}


void CLine::OnSetRect()
{
	SetPoint1(Gdiplus::Point(_rect.GetLeft(), _rect.GetTop()));
	SetPoint2(Gdiplus::Point(_rect.GetRight(), _rect.GetBottom()));

}

CLine::CLine(const Gdiplus::Point& point1, const Gdiplus::Point& point2)
{
	_point1 = point1;
	_point2 = point2;
	SetRect(Gdiplus::Rect(point1.X, point1.Y, abs(point2.X - point1.X), abs(point2.Y - point1.Y)));
}

CLine::~CLine()
{
}

void CLine::Draw(Gdiplus::Graphics& graphics)
{
	Pen pen(GetBorderColor());
	graphics.DrawLine(&pen, _point1, _point2);

	DrawBorder(graphics);
}

void CLine::Save(CArchive& ar)
{
	ar << int(ShapeLine);
	CShape::Save(ar);
	ar << _point1.X << _point1.Y << _point2.X << _point2.Y;
}

void CLine::Load(CArchive& ar)
{
	CShape::Load(ar);
 	ar >> _point1.X >> _point1.Y >> _point2.X >> _point2.Y;
}

int CLine::HitTest(const Gdiplus::Point& point)
{
	double a = abs(point.X - _point1.X);
	double b = abs(point.Y - _point1.Y);
	double c = abs(point.X - _point2.X);
	double d = abs(point.Y - _point2.Y);
	double w = abs(_point2.X - _point1.X);
	double h = abs(_point2.Y - _point1.Y);

	double line_value = sqrt(a*a + b*b) + sqrt(c*c + d*d);
	double line_value2 = sqrt(w*w + h*h);
	double s = fabs(line_value - line_value2);

	
	if (s>=1 && !_selected)
	{
		return HandleNone;
	}
	else
	{
		return CShape::HitTest(point);
	}
	
}

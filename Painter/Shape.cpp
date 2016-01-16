#include "stdafx.h"
#include "Shape.h"
#include <Gdiplus.h>
#include "..\Utilities\xml.h"

using namespace Gdiplus;


CShape::CShape() :
	_border_color(Gdiplus::Color(0, 255, 0)),
	_fill_color(Gdiplus::Color(192, 255, 192)),
	_selected(false)
{
}


CShape::~CShape()
{
}

void CShape::SetRect(const Gdiplus::Rect& rect)
{
	_rect = rect;
	OnSetRect();
}

const Gdiplus::Rect& CShape::GetRect() const
{	
	return _rect;
}

void CShape::Save(CArchive& ar)
{
	ar << _rect.GetLeft() << _rect.GetTop()
		<< _rect.Width << _rect.Height;

	ar << _fill_color.GetValue() << _border_color.GetValue();
}

void CShape::Load(CArchive& ar)
{
	Point top_left;
	Size size;
	ar >> top_left.X >> top_left.Y >> size.Width >> size.Height;

	_rect = Gdiplus::Rect(top_left, size);

	ARGB color;
	ar >> color;
	_fill_color = Color(color);

	ar >> color;
	_border_color = Color(color);
}

void CShape::Save(Utilities::CXmlElement& element)
{
	element.SetIntegerAttrib(LEFT, _rect.GetLeft());
	element.SetIntegerAttrib(TOP, _rect.GetTop());
	element.SetIntegerAttrib(WIDTH, _rect.Width);
	element.SetIntegerAttrib(HEIGHT, _rect.Height);

	element.SetIntegerAttrib(BORDERCOLOR, int(_border_color.ToCOLORREF()));
	element.SetIntegerAttrib(FILLCOLOR, int(_fill_color.ToCOLORREF()));
}

void CShape::Load(Utilities::CXmlElement& element)
{
	auto left = element.GetIntegerAttrib(LEFT);
	auto top = element.GetIntegerAttrib(TOP);
	auto width = element.GetIntegerAttrib(WIDTH);
	auto height = element.GetIntegerAttrib(HEIGHT);
	_rect = Rect(left, top, width, height);

	auto border_color = element.GetIntegerAttrib(BORDERCOLOR);
	_border_color.SetFromCOLORREF(border_color);

	auto fill_color = element.GetIntegerAttrib(FILLCOLOR);
	_fill_color.SetFromCOLORREF(fill_color);
}

void CShape::SetBorderColor(Gdiplus::Color border_color)
{
	_border_color = border_color;
}

Gdiplus::Color CShape::GetBorderColor() const
{
	return _border_color;
}

void CShape::SetFillColor(Gdiplus::Color fill_color)
{
	_fill_color = fill_color;
}

Gdiplus::Color CShape::GetFillColor() const
{
	return _fill_color;
}

bool CShape::HandleTest(const Point& center, const Gdiplus::Point& point)
{
	if (abs(center.X - point.X) < HANDLE_SIZE && abs(center.Y - point.Y) < HANDLE_SIZE)
	{
		return true;
	}

	return false;
}

int CShape::HitTest(const Gdiplus::Point& point)
{
	if (_selected)
	{
		if (HandleTest(Point(_rect.X, _rect.Y), point))
			return HandleTopLeft;
		else if (HandleTest(Point(_rect.X + _rect.Width / 2, _rect.Y), point))
			return HandleTopMiddle;
		else if (HandleTest(Point(_rect.X + _rect.Width, _rect.Y), point))
			return HandleTopRight;
		else if (HandleTest(Point(_rect.X, _rect.Y + _rect.Height / 2), point))
			return HandleMiddleLeft;
		else if (HandleTest(Point(_rect.X + _rect.Width, _rect.Y + _rect.Height / 2), point))
			return HandleMiddleRight;
		else if (HandleTest(Point(_rect.X, _rect.Y + _rect.Height), point))
			return HandleBottomLeft;
		else if (HandleTest(Point(_rect.X + _rect.Width / 2, _rect.Y + _rect.Height), point))
			return HandleBottomMiddle;
		else if (HandleTest(Point(_rect.X + _rect.Width, _rect.Y + _rect.Height), point))
			return HandleBottomRight;
	}

	if (_rect.Contains(point))
	{
		return HandleMove;
	}
	else
	{
		return HandleNone;
	}
}

void CShape::Select(bool select)
{
	_selected = select;
}

bool CShape::IsSelected() const
{
	return _selected;
}

void CShape::NormalizeRect(Gdiplus::Rect& rect)
{
	if (rect.Width < 0)
	{
		rect.X += rect.Width;
		rect.Width = -rect.Width;
	}
	if (rect.Height < 0)
	{
		rect.Y += rect.Height;
		rect.Height = -rect.Height;
	}
}

void CShape::DrawBorder(Gdiplus::Graphics& graphics)
{
	Rect rect = _rect;
	NormalizeRect(rect);

	if (_selected)
	{
		Pen pen(Color::Black);

		graphics.DrawRectangle(&pen, rect);

		// draw handles
		DrawHandle(graphics, pen, rect.GetLeft(), rect.GetTop());
		DrawHandle(graphics, pen, rect.GetRight(), rect.GetTop());
		DrawHandle(graphics, pen, rect.GetRight(), rect.GetBottom());
		DrawHandle(graphics, pen, rect.GetLeft(), rect.GetBottom());

		DrawHandle(graphics, pen, rect.GetRight(), (rect.GetTop() + rect.GetBottom()) / 2);
		DrawHandle(graphics, pen, rect.GetLeft(), (rect.GetTop() + rect.GetBottom()) / 2);
		DrawHandle(graphics, pen, (rect.GetLeft() + rect.GetRight()) / 2, rect.GetTop());
		DrawHandle(graphics, pen, (rect.GetLeft() + rect.GetRight()) / 2, rect.GetBottom());

		
	}	
}

void CShape::DrawHandle(Graphics& graphics, Pen& pen, INT x, INT y)
{
	Rect rect;
	rect.X = x - (HANDLE_SIZE - 1) / 2;
	rect.Y = y - (HANDLE_SIZE - 1) / 2;
	rect.Height = rect.Width = HANDLE_SIZE;

	SolidBrush brush(Color::White);
	graphics.FillRectangle(&brush, rect);
	graphics.DrawRectangle(&pen, rect);
}

void CShape::Move(int handle, int cx, int cy)
{
	switch (handle)
	{
	case HandleMove:
		_rect.X += cx;
		_rect.Y += cy;
		break;
	case HandleTopLeft:
		_rect.X += cx;
		_rect.Y += cy;
		_rect.Width -= cx;
		_rect.Height -= cy;
		break;
	case HandleTopMiddle:
		_rect.Y += cy;
		_rect.Height -= cy;
		break;
	case HandleTopRight:
		_rect.Y += cy;
		_rect.Width += cx;
		_rect.Height -= cy;
		break;
	case HandleMiddleLeft:
		_rect.X += cx;
		_rect.Width -= cx;
		break;
	case HandleMiddleRight:
		_rect.Width += cx;
		break;
	case HandleBottomLeft:
		_rect.X += cx;
		_rect.Width -= cx;
		_rect.Height += cy;
		break;
	case HandleBottomMiddle:
		_rect.Height += cy;
		break;
	case HandleBottomRight:
		_rect.Width += cx;
		_rect.Height += cy;
		break;

	default:
		;
	}
	OnSetRect();
}

void CShape::OnEndMove()
{
	NormalizeRect(_rect);
}

const CShape& CShape::operator = (const CShape& rhs)
{
	_rect = rhs._rect;
	_border_color = rhs._border_color;
	_fill_color = rhs._fill_color;
	_selected = rhs._selected;

	return *this;
}

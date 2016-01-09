#include "stdafx.h"
#include "Polygon.h"

using namespace Gdiplus;

CPolygon::CPolygon()
{
}


CPolygon::~CPolygon()
{
}

void CPolygon::Draw(Gdiplus::Graphics& graphics)
{
	Pen pen(GetBorderColor());
	if (_points.size() < 3)
	{
		ASSERT(_points.size() == 2);
		graphics.DrawLine(&pen, _points[0], _points[1]);
	}
	else
	{
		SolidBrush brush(GetFillColor());

		graphics.FillPolygon(&brush, _points.data(), _points.size());
		graphics.DrawPolygon(&pen, _points.data(), _points.size());
	}

	DrawBorder(graphics);
}

void CPolygon::Save(CArchive& ar)
{
	ar << (int)ShapePolygon;
	CShape::Save(ar);
	ar << _points.size();
	for (size_t i = 0; i < _points.size(); ++i)
	{
		ar << _points[i].X << _points[i].Y;
	}
}

void CPolygon::Load(CArchive& ar)
{
	CShape::Load(ar);
	size_t point_count = 0;
	ar >> point_count;

	_points.resize(point_count);
	for (size_t i = 0; i < point_count; ++i)
	{
		ar >> _points[i].X >> _points[i].Y;
	}
}

void CPolygon::AddPoint(Gdiplus::Point point)
{
	_points.push_back(point);
}

void CPolygon::SetPoint(unsigned int index, Gdiplus::Point point)
{
	if (index < _points.size())
	{
		_points[index] = point;
		SetRect();
	}
}

unsigned int CPolygon::GetPointCount() const
{
	return _points.size();
}

void CPolygon::SetRect()
{
}

void CPolygon::Finalize()
{
	Gdiplus::Point point1(_points[1].X, _points[1].Y);
	Gdiplus::Point point2(_points[1].X, _points[1].Y);

	for (size_t i = 0; i < _points.size(); i++)
	{
		if (point2.X < _points[i].X)
		{
			point2.X = _points[i].X;
		}

		if (point2.Y < _points[i].Y)
		{
			point2.Y = _points[i].Y;
		}

		if (point1.X > _points[i].X)
		{
			point1.X = _points[i].X;
		}

		if (point1.Y > _points[i].Y)
		{
			point1.Y = _points[i].Y;
		}
	}

	_rect = Rect(point1.X, point1.Y, abs(point2.X - point1.X), abs(point2.Y - point1.Y));

	_relative_points.resize(_points.size());
	for (size_t i = 0; i < _points.size(); ++i)
	{
		_relative_points[i].X = (_points[i].X - _rect.X) / (REAL)_rect.Width;
		_relative_points[i].Y = (_points[i].Y - _rect.Y) / (REAL)_rect.Height;
	}
}

void CPolygon::OnSetRect()
{
	_points.resize(_relative_points.size());
	for (size_t i = 0; i < _relative_points.size(); ++i)
	{
		_points[i].X = int(_relative_points[i].X * _rect.Width + _rect.X);
		_points[i].Y = int(_relative_points[i].Y * _rect.Height + _rect.Y);
	}
}

void CPolygon::OnEndMove()
{
	if (_rect.Width < 0)
	{
		for (size_t i = 0; i < _relative_points.size(); ++i)
		{
			_relative_points[i].X = 1.0f - _relative_points[i].X;
		}
	}
	if (_rect.Height < 0)
	{
		for (size_t i = 0; i < _relative_points.size(); ++i)
		{
			_relative_points[i].Y = 1.0f - _relative_points[i].Y;
		}
	}

	NormalizeRect(_rect);
}

void CPolygon::DrawBorder(Gdiplus::Graphics& graphics)
{

	Pen pen(Color::Black);
	if (_selected)
	{
		for (size_t i = 0; i < _points.size(); ++i)
		{
			DrawHandle(graphics, pen, _points[i].X, _points[i].Y);
		}
	}

	CShape::DrawBorder(graphics);
}

int CPolygon::HitTest(const Gdiplus::Point& point)
{
	if (_selected)
	{
		for (size_t i = 0; i < _points.size(); ++i)
		{
			if (HandleTest(Gdiplus::Point(_points[i].X, _points[i].Y), point))
			{
				_handle_point.X = _points[i].X;
				_handle_point.Y = _points[i].Y;
				return HandlePoint;
			}		
		}
	}

	return CShape::HitTest(point);
}

void CPolygon::Move(int handle_to_move, int cx, int cy)
{
	if (handle_to_move == HandlePoint)
	{
		for (size_t i = 0; i < _points.size(); ++i)
		{
			if (_points[i].X == _handle_point.X && _points[i].Y == _handle_point.Y)
			{
				_points[i].X += cx;
				_points[i].Y += cy;
			}
		}
	}

	OnSetRect();

	CShape::Move(handle_to_move, cx, cy);
}


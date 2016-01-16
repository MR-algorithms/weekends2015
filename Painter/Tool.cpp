#include "stdafx.h"
#include "Tool.h"

IShapeUser* CTool::s_shape_user = nullptr;

CTool::CTool() :
_border_color(Gdiplus::Color::Black),
_fill_color(Gdiplus::Color::White)
{
}


CTool::~CTool()
{
}

void CTool::SetBorderColor(Gdiplus::Color color)
{
	_border_color = color;
}

Gdiplus::Color CTool::GetBorderColor() const
{
	return _border_color;
}

Gdiplus::Color CTool::GetFillColor() const
{
	return _fill_color;
}

void CTool::SetFillColor(Gdiplus::Color color)
{
	_fill_color = color;
}

bool CTool::OnLButtonDoubleClick(UINT nFlags, CPoint point)
{
	return false;
}

void CTool::SetShapeUser(IShapeUser * shape_user)
{
	ASSERT(shape_user != nullptr);
	s_shape_user = shape_user;
}

bool CTool::OnLButtonUp(UINT nFlags, CPoint point)
{
	return false;
}

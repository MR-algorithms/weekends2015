#include "stdafx.h"
#include "EllipseTool.h"
#include "Ellipse.h"
#include <gdiplus.h>

using namespace std;
using namespace Gdiplus;


CEllipseTool::CEllipseTool()
{
}


CEllipseTool::~CEllipseTool()
{
}

bool CEllipseTool::OnLButtonDown(UINT nFlags, CPoint point)
{
	_down_point = point;
	_ellipse = shared_ptr<CEllipse>(new CEllipse(Gdiplus::Point(point.x, point.y),
		Gdiplus::Point(point.x, point.y)));

	_ellipse->SetBorderColor(GetBorderColor());
	_ellipse->SetFillColor(GetFillColor());

	return true;
}

bool CEllipseTool::OnMouseMove(UINT nFlags, CPoint point)
{
	if ((nFlags & MK_LBUTTON) == MK_LBUTTON && _ellipse)
	{
		CRect rect(_down_point, point);
		rect.NormalizeRect();

		_ellipse->SetRect(Gdiplus::Rect(rect.left, rect.top, rect.Width(), rect.Height()));

		return true;
	}

	return false;
}

std::shared_ptr<CShape> CEllipseTool::GetShape()
{
	return _ellipse;
}

bool CEllipseTool::OnLButtonUp(UINT nFlags, CPoint point)
{
	s_shape_user->AddShape(_ellipse);

	return false;
}

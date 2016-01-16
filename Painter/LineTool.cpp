#include "stdafx.h"

#include "LineTool.h"
#include "line.h"
#include <gdiplus.h>

using namespace std;
using namespace Gdiplus;

CLineTool::CLineTool()
{
}


CLineTool::~CLineTool()
{
}

bool CLineTool::OnLButtonDown(UINT nFlags, CPoint point)
{
	_line = shared_ptr<CLine>(new CLine(Point(point.x, point.y),
		 			Point(point.x, point.y)));
	_line->SetBorderColor(GetBorderColor());

	return true;
}

bool CLineTool::OnMouseMove(UINT nFlags, CPoint point)
{
	if ((nFlags & MK_LBUTTON) == MK_LBUTTON && _line)
	{
		_line->SetPoint2(Point(point.x, point.y));

		Gdiplus::Point p1 = _line->GetPoint1();
		CPoint point1(p1.X, p1.Y);
		CRect rect(point1, point);
		//rect.NormalizeRect();

		_line->SetRect(Gdiplus::Rect(rect.left, rect.top, rect.Width(), rect.Height()));

		return true;
	}

	return false;
}

std::shared_ptr<CShape> CLineTool::GetShape()
{
	return _line;
}

bool CLineTool::OnLButtonUp(UINT nFlags, CPoint point)

{
	
	s_shape_user->AddShape(_line);
	

	return false;

	
}

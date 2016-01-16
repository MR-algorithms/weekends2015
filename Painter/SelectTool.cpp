#include "stdafx.h"
#include "SelectTool.h"
#include <gdiplus.h>
#include "Shape.h"

using namespace std;
using namespace Gdiplus;

CSelectTool::CSelectTool() :
	_handle_hit(HandleNone)
{
}


CSelectTool::~CSelectTool()
{
}

bool CSelectTool::OnLButtonDown(UINT nFlags, CPoint point)
{
	const auto & shapes = s_shape_user->Shapes();

	for (auto iter = shapes.rbegin(); iter != shapes.rend(); ++iter)
	{
		bool object_selected = false;
		auto handle = (*iter)->HitTest(Point(point.x, point.y));
		if (handle != HandleNone && !object_selected)
		{
			(*iter)->Select(true);
			_handle_hit = handle;
			object_selected = true;

			break;
		}
		else if ((nFlags & MK_CONTROL) != MK_CONTROL)
		{
			(*iter)->Select(false);
		}
	}

	_last_point = point;

	return true;
}

bool CSelectTool::OnMouseMove(UINT nFlags, CPoint point)
{
	if ((nFlags & MK_LBUTTON) == MK_LBUTTON)
	{
		CSize offset = point - _last_point;
		const auto & shapes = s_shape_user->Shapes();
		for (auto iter = shapes.begin(); iter != shapes.end(); ++iter)
		{
			if ((*iter)->IsSelected())
			{
				(*iter)->Move(_handle_hit, offset.cx, offset.cy);
			}
		}

		_last_point = point;

		return true;
	}

	return false;
}

std::shared_ptr<CShape> CSelectTool::GetShape()
{
	return shared_ptr<CShape>();
}

bool CSelectTool::OnLButtonUp(UINT nFlags, CPoint point)
{
	const auto & shapes = s_shape_user->Shapes();
	for (auto iter = shapes.begin(); iter != shapes.end(); ++iter)
	{
		if ((*iter)->IsSelected())
		{
			(*iter)->OnEndMove();
		}
	}

	return true;
}

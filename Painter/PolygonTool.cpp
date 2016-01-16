#include "stdafx.h"
#include "PolygonTool.h"
#include "Polygon.h"

using namespace std;
using namespace Gdiplus;

CPolygonTool::CPolygonTool()
{
}


CPolygonTool::~CPolygonTool()
{
}

bool CPolygonTool::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!_polygon)
	{
		_polygon = shared_ptr<CPolygon>(new CPolygon);
		_polygon->AddPoint(Point(point.x, point.y));
		_polygon->AddPoint(Point(point.x, point.y));
	}
	else
	{
		_polygon->AddPoint(Point(point.x, point.y));
	}

	_polygon->SetBorderColor(GetBorderColor());
	_polygon->SetFillColor(GetFillColor());

	return true;
}

bool CPolygonTool::OnMouseMove(UINT nFlags, CPoint point)
{
	if (_polygon)
	{
		_polygon->SetPoint(_polygon->GetPointCount() - 1, Point(point.x, point.y));
		return true;
	}

	return false;
}

bool CPolygonTool::OnLButtonDoubleClick(UINT nFlags, CPoint point)
{
	if (_polygon)
	{
		_polygon->Finalize();
		s_shape_user->AddShape(_polygon);
		_polygon.reset();
	}

	return false;
}

std::shared_ptr<CShape> CPolygonTool::GetShape()
{
	return _polygon;
}

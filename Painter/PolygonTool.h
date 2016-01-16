#pragma once
#include "Tool.h"

class CPolygon;

class CPolygonTool :
	public CTool
{
public:
	CPolygonTool();
	virtual ~CPolygonTool();

	virtual bool OnLButtonDown(UINT nFlags, CPoint point);
	virtual bool OnMouseMove(UINT nFlags, CPoint point);
	virtual bool OnLButtonDoubleClick(UINT nFlags, CPoint point);

	virtual std::shared_ptr<CShape> GetShape();

protected:
	std::shared_ptr<CPolygon> _polygon;
};


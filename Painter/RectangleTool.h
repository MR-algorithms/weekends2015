#pragma once
#include "Tool.h"
#include <memory>

class CRectangle;

class CRectangleTool :
	public CTool
{
public:
	CRectangleTool();
	virtual ~CRectangleTool();
	virtual bool OnLButtonDown(UINT nFlags, CPoint point);
	virtual bool OnMouseMove(UINT nFlags, CPoint point);
	virtual bool OnLButtonUp(UINT nFlags, CPoint point);

	virtual std::shared_ptr<CShape> GetShape();

private:
	std::shared_ptr<CRectangle> _rect;
	CPoint _down_point;
};



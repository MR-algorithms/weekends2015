#pragma once
#include "Tool.h"
#include <memory>

class CEllipse;

class CEllipseTool :
	public CTool
{
public:
	CEllipseTool();
	virtual ~CEllipseTool();

	virtual bool OnLButtonDown(UINT nFlags, CPoint point);
	virtual bool OnMouseMove(UINT nFlags, CPoint point);
	virtual bool OnLButtonUp(UINT nFlags, CPoint point);
	virtual std::shared_ptr<CShape> GetShape();

private:
	std::shared_ptr<CEllipse> _ellipse;
	CPoint _down_point;
};


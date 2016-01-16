#pragma once
#include "Tool.h"
#include <memory>

class CLine;

class CLineTool :
	public CTool
{
public:
	CLineTool();
	virtual ~CLineTool();

	virtual bool OnLButtonDown(UINT nFlags, CPoint point);
	virtual bool OnMouseMove(UINT nFlags, CPoint point);
	virtual bool OnLButtonUp(UINT nFlags, CPoint point);
	virtual std::shared_ptr<CShape> GetShape();

private:
	std::shared_ptr<CLine> _line;
};


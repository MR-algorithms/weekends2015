#pragma once
#include "Tool.h"
class CSelectTool :
	public CTool
{
public:
	CSelectTool();
	virtual ~CSelectTool();

	virtual bool OnLButtonDown(UINT nFlags, CPoint point) override;
	virtual bool OnMouseMove(UINT nFlags, CPoint point) override;

	virtual std::shared_ptr<CShape> GetShape() override;

	virtual bool OnLButtonUp(UINT nFlags, CPoint point) override;

private:
	CPoint _last_point;
	int _handle_hit;
};


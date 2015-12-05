#pragma once

#include <memory>
class CShape;

struct IShapeUser
{
	virtual bool AddShape(std::shared_ptr<CShape> shape) = 0;
};

class CTool
{
public:
	CTool();
	virtual ~CTool();

	virtual void OnLButtonDown(UINT nFlags, CPoint point) = 0;
	virtual void OnMouseMove(UINT nFlags, CPoint point) = 0;
	virtual void OnLButtonUp(UINT nFlags, CPoint point);

	virtual void OnLButtonDoubleClick(UINT nFlags, CPoint point);

	virtual std::shared_ptr<CShape> GetShape() = 0;

	void SetBorderColor(Gdiplus::Color color);
	Gdiplus::Color GetBorderColor() const;

	void SetFillColor(Gdiplus::Color color);
	Gdiplus::Color GetFillColor() const;

	static void SetShapeUser(IShapeUser * shape_user);

protected:
	static IShapeUser* s_shape_user;

	Gdiplus::Color _fill_color;
	Gdiplus::Color _border_color;
};


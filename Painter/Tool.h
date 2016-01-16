#pragma once

#include <memory>
#include <vector>

class CShape;

struct IShapeUser
{
	virtual bool AddShape(std::shared_ptr<CShape> shape) = 0;
	virtual const std::vector<std::shared_ptr<CShape>> & Shapes() const = 0;
};

class CTool
{
public:
	CTool();
	virtual ~CTool();

	virtual bool OnLButtonDown(UINT nFlags, CPoint point) = 0;
	virtual bool OnMouseMove(UINT nFlags, CPoint point) = 0;
	virtual bool OnLButtonUp(UINT nFlags, CPoint point);

	virtual bool OnLButtonDoubleClick(UINT nFlags, CPoint point);

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


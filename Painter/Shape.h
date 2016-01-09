#pragma once

#include <Gdiplus.h>

namespace Utilities { class CXmlElement; }

enum ShapeType
{
	ShapeLine = 1,
	ShapeRectangle = 2,
	ShapeEllipse = 3,
	ShapePolygon = 4,
};

const int HandleNone = 0;
const int HandleMove = 1;
const int HandleRotate = 2;
const int HandleTopLeft = 3;
const int HandleTopMiddle = 4;
const int HandleTopRight = 5;
const int HandleMiddleLeft = 6;
const int HandleMiddleRight = 7;
const int HandleBottomLeft = 8;
const int HandleBottomMiddle = 9;
const int HandleBottomRight = 10;
const int HandlePoint = 11;

class CShape
{
public:
	CShape();
	virtual ~CShape();

	virtual void Draw(Gdiplus::Graphics& graphics) = 0;

	virtual void Save(CArchive& ar);
	virtual void Load(CArchive& ar);
	virtual void Save(Utilities::CXmlElement& element);
	virtual void Load(Utilities::CXmlElement& element);

	virtual int HitTest(const Gdiplus::Point& point);
	virtual void Move(int handle_to_move, int cx, int cy);
	virtual void OnEndMove();
	virtual void OnSetRect(){}
	

	void SetRect(const Gdiplus::Rect& rect);
	const Gdiplus::Rect& GetRect() const;
	void SetBorderColor(Gdiplus::Color border_color);
	Gdiplus::Color GetBorderColor() const;
	void SetFillColor(Gdiplus::Color fill_color);
	Gdiplus::Color GetFillColor() const;
	void Select(bool select);
	bool IsSelected() const;

protected:
	virtual void DrawBorder(Gdiplus::Graphics& graphics);
	void DrawHandle(Gdiplus::Graphics& graphics, Gdiplus::Pen&pen, INT x, INT y);
	bool HandleTest(const Gdiplus::Point& center, const Gdiplus::Point& point);
	void NormalizeRect(Gdiplus::Rect& rect);

	Gdiplus::Rect _rect;
	Gdiplus::Color _border_color;
	Gdiplus::Color _fill_color;
	bool _selected;
	const int HANDLE_SIZE = 7;
};


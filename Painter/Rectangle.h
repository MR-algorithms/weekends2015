#pragma once
#include <gdiplus.h>
#include "Shape.h"

class CRectangle : public CShape
{
public:
	CRectangle();
	CRectangle(INT x, INT y, INT width, INT height);

	virtual ~CRectangle();
	INT GetX() const{
		return _top_left.X;
	}
	void SetX(INT x){
		_top_left.X = x;
	}

	INT GetY() const{
		return _top_left.Y;
	}

	void SetY(INT y){
		_top_left.Y = y;
	}

	INT GetWidth() const{
		return _size.Width;
	}

	void SetWidth(INT width){
		_size.Width = width;
	}

	INT GetHeight() const {
		return _size.Height;
	}

	void SetHeight(INT height){
		_size.Height = height;
	}

	virtual void Draw(Gdiplus::Graphics& graphics);
	virtual void Save(CArchive& ar);

	virtual void Save(Utilities::CXmlElement& element) override;

	virtual void Load(Utilities::CXmlElement& element) override;

	virtual CShape * Clone() override;

private:
	Gdiplus::Point _top_left;
	Gdiplus::Size _size;
};



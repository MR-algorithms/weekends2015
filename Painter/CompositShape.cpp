#include "stdafx.h"
#include "CompositShape.h"
#include <iterator>
#include "..\Utilities\xml.h"
#include "PainterDoc.h"
#include "Shape.h"
#include <memory>
#include "Polygon.h"
#include "Line.h"
#include "Ellipse.h"
#include "Rectangle.h"
#include <gdiplus.h>
#include "ShapeFactory.h"

using namespace Gdiplus;

const TCHAR * RELATIVE_LEFT = _T("RelativeLeft");
const TCHAR * RELATIVE_TOP = _T("RelativeTop");
const TCHAR * RELATIVE_WIDHT = _T("RelativeWidth");
const TCHAR * RELATIVE_HEIGHT = _T("RelativeHeight");
const TCHAR * SHAPECOUNT = _T("ShapeCount");
const TCHAR * COMPOSITECONTENT = _T("CompositeContent");

CCompositShape::CCompositShape()
{
}


CCompositShape::~CCompositShape()
{
}

void CCompositShape::Draw(Gdiplus::Graphics& graphics)
{
	for (auto iter = _children.begin(); iter != _children.end(); ++iter)
	{
		(*iter)->Draw(graphics);
	}

	DrawBorder(graphics);
}

void CCompositShape::Save(CArchive& ar)
{
	ar << (int)ShapeComposite;
	CShape::Save(ar);
	for (size_t i = 0; i < _children.size();++i)
	{
		_children[i]->Save(ar);
	}

	
}

void CCompositShape::Load(CArchive& ar)
{
	CShape::Load(ar);
	size_t shape_count;
	ar >> shape_count;
	_children.resize(shape_count);
	for (size_t i = 0; i < shape_count; ++i)
	{
		__super::Load(ar);
	}
}

void CCompositShape::Save(Utilities::CXmlElement& element)
{
	element.SetAttrib(TYPE, COMPOSITE);
	element.SetIntegerAttrib(SHAPECOUNT, _children.size());
	element.SetFloatAttrib(LEFT, _rect.GetLeft());
	element.SetFloatAttrib(TOP, _rect.GetTop());
	element.SetFloatAttrib(WIDTH, _rect.Width);
	element.SetFloatAttrib(HEIGHT,_rect.Height);

	for (size_t i = 0; i < _children.size();++i)
	{
		auto child_element = element.AddElement(COMPOSITECONTENT);
		_children[i]->Save(*child_element);
		child_element->SetFloatAttrib(RELATIVE_LEFT, _relative_postions[i].X);
		child_element->SetFloatAttrib(RELATIVE_TOP, _relative_postions[i].Y);
		child_element->SetFloatAttrib(RELATIVE_WIDHT, _relative_postions[i].Width);
		child_element->SetFloatAttrib(RELATIVE_HEIGHT, _relative_postions[i].Height);
	}
}


void CCompositShape::Load(Utilities::CXmlElement& element) 
{
	//firstly load the composite tap attribution
	__super::Load(element);
	size_t shape_count = element.GetIntegerAttrib(SHAPECOUNT);
	//load the child's elements
	auto composite_elment = element.GetChildElements();
	std::shared_ptr<CShape> shape;
	Gdiplus::RectF rectf;
	_children.clear();

	CShapeFactory shape_factory;

	for (unsigned int i = 0; i < shape_count; ++i)
	{
		auto type = composite_elment[i]->GetAttrib(TYPE);
		shape = shape_factory.CreateShape(type);
	
		auto X = composite_elment[i]->GetFloatAttrib(RELATIVE_LEFT);
		auto Y = composite_elment[i]->GetFloatAttrib(RELATIVE_TOP);
		auto Width = composite_elment[i]->GetFloatAttrib(RELATIVE_WIDHT);
		auto Height = composite_elment[i]->GetFloatAttrib(RELATIVE_HEIGHT);
		rectf = Gdiplus::RectF((REAL)X, (REAL)Y, (REAL)Width, (REAL)Height);
		_relative_postions.push_back(rectf);
		shape->Load(*composite_elment[i]);
		shape->OnSetRect();
		AddShape(shape);
	}
}


int CCompositShape::HitTest(const Gdiplus::Point& point)
{
	return __super::HitTest(point);
}

void CCompositShape::OnSetRect()
{
	for (size_t i = 0; i < _children.size(); ++i)
	{
		Rect child_rect;
		child_rect.X = INT(_relative_postions[i].X * _rect.Width + _rect.X);
		child_rect.Y = INT(_relative_postions[i].Y * _rect.Height + _rect.Y);
		child_rect.Width = INT(_relative_postions[i].Width * _rect.Width);
		child_rect.Height = INT(_relative_postions[i].Height * _rect.Height);

		_children[i]->SetRect(child_rect);
	}
}

bool CCompositShape::AddShape(std::shared_ptr<CShape> shape)
{
	try
	{
		_children.push_back(shape);
		return true;
	}
	catch (std::bad_alloc&)
	{
		return false;
	}
}

void CCompositShape::UpdateRelativePostions()
{
	if (_children.empty())
		return;

	_rect = _children[0]->GetRect();

	Point top_left, bottom_right;
	top_left.X = _rect.X;
	top_left.Y = _rect.Y;
	bottom_right.X = _rect.X + _rect.Width;
	bottom_right.Y = _rect.Y + _rect.Height;

	for (size_t i = 1; i < _children.size(); ++i)
	{
		const Rect& child_rect = _children[i]->GetRect();
		if (child_rect.X < top_left.X)
		{
			top_left.X = child_rect.X;
		}
		if (child_rect.Y < top_left.Y)
		{
			top_left.Y = child_rect.Y;
		}
		if (child_rect.X + child_rect.Width > bottom_right.X)
		{
			bottom_right.X = child_rect.X + child_rect.Width;
		}
		if (child_rect.Y + child_rect.Height > bottom_right.Y)
		{
			bottom_right.Y = child_rect.Y + child_rect.Height;
		}
	}
	_rect.X = top_left.X;
	_rect.Y = top_left.Y;
	_rect.Width = bottom_right.X - top_left.X;
	_rect.Height = bottom_right.Y - top_left.Y;

	_relative_postions.resize(_children.size());
	for (size_t i = 0; i < _children.size(); ++i)
	{
		const Rect& child_rect = _children[i]->GetRect();
		_relative_postions[i].X = (child_rect.X - _rect.X) / REAL(_rect.Width);
		_relative_postions[i].Y = (child_rect.Y - _rect.Y) / REAL(_rect.Height);
		_relative_postions[i].Width = child_rect.Width / REAL(_rect.Width);
		_relative_postions[i].Height = child_rect.Height / REAL(_rect.Height);
	}
}

void CCompositShape::Ungroup(std::vector<std::shared_ptr<CShape>>& parent_container)
{
	copy(_children.begin(), _children.end(), back_inserter(parent_container));
}

void CCompositShape::OnEndMove()
{
	if (_rect.Width < 0)
	{
		for (size_t i = 0; i < _relative_postions.size(); ++i)
		{
			_children[i]->OnEndMove();
			_relative_postions[i].X = (1.0f - _relative_postions[i].X) - _relative_postions[i].Width;
		}
	}
	if (_rect.Height < 0)
	{
		for (size_t i = 0; i < _relative_postions.size(); ++i)
		{
			_children[i]->OnEndMove();
			_relative_postions[i].Y = (1.0f - _relative_postions[i].Y) - _relative_postions[i].Height;
		}
	}

	NormalizeRect(_rect);
}

CShape * CCompositShape::Clone()
{
	CCompositShape * composite = new CCompositShape;
	*composite = *this;
	
	return composite;
}

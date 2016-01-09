#include "stdafx.h"
#include "CompositShape.h"
#include <iterator>

using namespace Gdiplus;

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
	throw std::logic_error("The method or operation is not implemented.");
}

void CCompositShape::Load(CArchive& ar)
{
	throw std::logic_error("The method or operation is not implemented.");
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
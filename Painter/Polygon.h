#pragma once
#include "Shape.h"

#include <vector>
#include <gdiplus.h>
#include <memory>
#include "..\Utilities\xml.h"

class CPolygon :
	public CShape
{
public:
	CPolygon();
	virtual ~CPolygon();

	virtual void Draw(Gdiplus::Graphics& graphics);
	virtual void Save(CArchive& ar);
	virtual void Load(CArchive& ar);
	virtual void OnSetRect();

	virtual void Save(Utilities::CXmlElement& element) override;
	virtual void Load(Utilities::CXmlElement& element) override;

	void AddPoint(Gdiplus::Point point);
	void SetPoint(unsigned int index, Gdiplus::Point point);
	unsigned int GetPointCount() const;
	void SetRect();
	void Finalize();

	virtual void OnEndMove() override;

	virtual void DrawBorder(Gdiplus::Graphics& graphics) override;

	virtual int HitTest(const Gdiplus::Point& point) override;

	virtual void Move(int handle_to_move, int cx, int cy) override;

private:
	std::vector<Gdiplus::Point> _points;
	std::vector<Gdiplus::PointF> _relative_points;
    Gdiplus::Point _handle_point;
};


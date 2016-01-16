#pragma once
#include <gdiplus.h>
#include "Shape.h"

class CLine : public CShape
{
public:
	CLine();
	CLine(const Gdiplus::Point& point1, const Gdiplus::Point& point2);
	virtual ~CLine();

	const Gdiplus::Point& GetPoint1() const{
		return _point1;
	}
	void SetPoint1(const Gdiplus::Point& point){
		_point1 = point;
	}

	const Gdiplus::Point& GetPoint2() const{
		return _point2;
	}
	void SetPoint2(const Gdiplus::Point& point){
		_point2 = point;
	}
	virtual void Draw(Gdiplus::Graphics& graphics);
	virtual void Save(CArchive& ar);
	virtual void Load(CArchive& ar);
	virtual void OnSetRect();
	virtual void Save(Utilities::CXmlElement& element) override;
	virtual void Load(Utilities::CXmlElement& element) override;
	virtual int HitTest(const Gdiplus::Point& point) override;
	virtual void DrawBorder(Gdiplus::Graphics& graphics) override;
	virtual CShape * Clone() override;
	const TCHAR * LINE = _T("Line");
private:
	Gdiplus::Point _point1;
	Gdiplus::Point _point2;
};

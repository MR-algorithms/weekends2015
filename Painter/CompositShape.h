#pragma once
#include "Shape.h"
#include <vector>
#include <memory>
#include <gdiplus.h>

class CShapeFactory;

class CCompositShape :
	public CShape
{
public:
	CCompositShape();
	virtual ~CCompositShape();

	bool AddShape(std::shared_ptr<CShape> shape);
	virtual void Draw(Gdiplus::Graphics& graphics) override;

	virtual void Save(CArchive& ar) override;

	virtual void Load(CArchive& ar) override;

	virtual void Save(Utilities::CXmlElement& element) override;
	virtual void Load(Utilities::CXmlElement& element) override;

	virtual int HitTest(const Gdiplus::Point& point) override;

	virtual void OnEndMove() override;
	virtual void OnSetRect() override;
	void Ungroup(std::vector<std::shared_ptr<CShape>>& parent_container);

	void UpdateRelativePostions();

	virtual CShape * Clone() override;

private:
	std::vector<std::shared_ptr<CShape>> _children;
	std::vector<Gdiplus::RectF> _relative_postions;
	std::shared_ptr<CShapeFactory>  _shape_factory;
};


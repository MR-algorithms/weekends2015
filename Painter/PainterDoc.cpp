// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// PainterDoc.cpp : implementation of the CPainterDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Painter.h"
#endif

#include "PainterDoc.h"

#include <propkey.h>
#include <memory>
#include "line.h"
#include <gdiplus.h>
#include "Rectangle.h"
#include "Polygon.h"
#include "Ellipse.h"
#include "CompositShape.h"
#include <iterator>
#include "../Utilities/xml.h"
#include "../Utilities/Clipboard.h"
#include "ShapeFactory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;
using namespace Gdiplus;
using namespace Utilities;
const TCHAR * RECTANGLE = _T("Rectangle");
const TCHAR * ELLIPSE = _T("Ellipse");
const TCHAR * LINE = _T("Line");
const TCHAR * POLYGON = _T("Polygon");
const TCHAR * COMPOSITE = _T("Composite");
const TCHAR * SHAPE = _T("Shape");
const TCHAR* SHAPES = _T("Shapes");
const TCHAR* COUNT = _T("Count");
const TCHAR * TYPE = _T("Type");
// CPainterDoc

IMPLEMENT_DYNCREATE(CPainterDoc, CDocument)

BEGIN_MESSAGE_MAP(CPainterDoc, CDocument)
END_MESSAGE_MAP()


// CPainterDoc construction/destruction

CPainterDoc::CPainterDoc()
{
	_shape_factory = shared_ptr<CShapeFactory>(new CShapeFactory);

	_shape_factory->InsertShape(RECTANGLE, shared_ptr<CShape>(new CRectangle));
	_shape_factory->InsertShape(ELLIPSE, shared_ptr<CShape>(new CEllipse));
	_shape_factory->InsertShape(LINE, shared_ptr<CShape>(new CLine));
	_shape_factory->InsertShape(POLYGON, shared_ptr<CShape>(new CPolygon));
	_shape_factory->InsertShape(COMPOSITE, shared_ptr<CShape>(new CCompositShape));
}

CPainterDoc::~CPainterDoc()
{
}

BOOL CPainterDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}


// CPainterDoc serialization

void CPainterDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
// 		ar << static_cast<size_t>(_shapes.size());
// 		for (auto shape = _shapes.begin(); shape != _shapes.end(); ++shape)
// 		{
// 			(*shape)->Save(ar);
// 		}
		CXml xml(SHAPE);
		xml.SetIntegerAttrib(COUNT, _shapes.size());

		for (auto shape : _shapes)
		{
			auto element = xml.AddElement(SHAPE);
			shape->Save(*element);
		}
		ar << xml.GetDoc();
	}
	else
	{
		_shapes.clear();

		CString xml_string;
		ar >> xml_string;

		CXml xml;
		xml.SetDoc(xml_string);

		unsigned count(xml.GetIntegerAttrib(COUNT));
		auto shape_elements = xml.GetChildElements();

		shared_ptr<CShape> shape;
		for (unsigned int i = 0; i < count; ++i)
		{
			auto type = shape_elements[i]->GetAttrib(TYPE);
			shape = _shape_factory->CreateShape(type);

			shape->Load(*shape_elements[i]);
			_shapes.push_back(shape);
		}
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CPainterDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CPainterDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CPainterDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CPainterDoc diagnostics

#ifdef _DEBUG
void CPainterDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CPainterDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


const vector<shared_ptr<CShape>>& CPainterDoc::GetShapes() const
{
	return _shapes;
}

std::vector<std::shared_ptr<CShape>>& CPainterDoc::Shapes()
{
	return _shapes;
}

void CPainterDoc::AddShape(shared_ptr<CShape> shape)
{
	_shapes.push_back(shape);
}

bool CPainterDoc::Group()
{
	shared_ptr<CCompositShape> composite(new CCompositShape);

	for (auto iter = _shapes.begin(); iter != _shapes.end();)
	{
		if ((*iter)->IsSelected())
		{
			(*iter)->Select(false);
			composite->AddShape(*iter);
			iter = _shapes.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	composite->UpdateRelativePostions();
	composite->Select(true);
	_shapes.push_back(composite);

	return true;
}

bool CPainterDoc::Ungroup()
{
	vector<shared_ptr<CShape>> shapes_collected;
	for (auto iter = _shapes.begin(); iter != _shapes.end(); )
	{
		if ((*iter)->IsSelected())
		{
			CCompositShape * composite = dynamic_cast<CCompositShape*>((*iter).get());
			if (composite != nullptr)
			{
				composite->Ungroup(shapes_collected);
				iter = _shapes.erase(iter);
				continue;
			}
		}
		
		++iter;
	}
	for (auto shape : shapes_collected)
	{
		shape->Select(true);
	}
	copy(shapes_collected.begin(), shapes_collected.end(), back_inserter(_shapes));

	return true;
}

bool CPainterDoc::Cut()
{
	for (auto iter = _shapes.begin(); iter != _shapes.end();)
	{
		if ((*iter)->IsSelected())
		{
			(*iter)->Select(false);
			iter = _shapes.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	return true;
}



void CPainterDoc::Copy()
{
	CXml xml(SHAPES);

	for (auto shape : _shapes)
	{
		if (shape->IsSelected())
		{
			auto child_element = xml.AddElement(SHAPE);
			shape->Save(*child_element);
		}
	}

	CClipboard::SetClipboardText(xml.GetDoc());
}

void CPainterDoc::Paste()
{
	auto contents = CClipboard::GetClipboardText();
	CXml xml;
	xml.SetDoc(contents);

	auto children = xml.GetChildElements();

	shared_ptr<CShape> shape;
	for (auto child : children)
	{
		auto type = child->GetAttrib(TYPE);
		shape = _shape_factory->CreateShape(type);

		shape->Load(*child);
		_shapes.push_back(shape);
	}
}

// CPainterDoc commands

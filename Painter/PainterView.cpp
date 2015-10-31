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

// PainterView.cpp : implementation of the CPainterView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Painter.h"
#endif

#include "PainterDoc.h"
#include "PainterView.h"
#include <Gdiplus.h>

#include "line.h"
#include "Rectangle.h"

using namespace std;
using namespace Gdiplus;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPainterView

IMPLEMENT_DYNCREATE(CPainterView, CView)

BEGIN_MESSAGE_MAP(CPainterView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPainterView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
//	ON_WM_MOVE()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_BUTTON_LINE, &CPainterView::OnButtonLine)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_LINE, &CPainterView::OnUpdateButtonLine)
	ON_COMMAND(ID_BUTTON_RECTANGLE, &CPainterView::OnButtonRectangle)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_RECTANGLE, &CPainterView::OnUpdateButtonRectangle)
END_MESSAGE_MAP()

// CPainterView construction/destruction

CPainterView::CPainterView() :
	_tool(ToolTypeLine)
{
}

CPainterView::~CPainterView()
{
}

BOOL CPainterView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CPainterView drawing

void CPainterView::OnDraw(CDC* pDC)
{
	Gdiplus::Graphics graphics(pDC->m_hDC);

	CRect rect;
	GetClientRect(rect);

	Gdiplus::Bitmap bitmap(rect.Width(), rect.Height(), &graphics);
	Gdiplus::Graphics buffer_graphics(&bitmap);

	Draw(buffer_graphics);
	graphics.DrawImage(&bitmap, rect.left, rect.top, rect.right, rect.bottom);
}

void CPainterView::Draw(Gdiplus::Graphics& graphics)
{
	CPainterDoc* doc = GetDocument();
	ASSERT_VALID(doc);
	if (!doc)
		return;

	CRect rect;
	GetClientRect(rect);

	SolidBrush BKbrush(Gdiplus::Color::White);
	graphics.FillRectangle(&BKbrush, 0, 0, rect.Width(), rect.Height());

	auto& shapes = doc->GetShapes();

	for (auto shape = shapes.begin(); shape != shapes.end(); ++shape)
	{
		(*shape)->Draw(graphics);
	}

	if (_temp_line)
	{
		_temp_line->Draw(graphics);
	}
	if (_temp_rect)
	{
		_temp_rect->Draw(graphics);

	}
}


// CPainterView printing


void CPainterView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CPainterView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPainterView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPainterView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CPainterView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPainterView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CPainterView diagnostics

#ifdef _DEBUG
void CPainterView::AssertValid() const
{
	CView::AssertValid();
}

void CPainterView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPainterDoc* CPainterView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPainterDoc)));
	return (CPainterDoc*)m_pDocument;
}
#endif //_DEBUG


// CPainterView message handlers


void CPainterView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (_tool == ToolTypeLine)
	{
		_temp_line = shared_ptr<CLine>(new CLine(Point(point.x, point.y),
			Point(point.x, point.y)));
	}
	else if (_tool == ToolTypeRectangle)
	{
		_temp_rect = shared_ptr<CRectangle>(new CRectangle(point.x, point.y, 0, 0));
	}

	Invalidate(FALSE);
	UpdateWindow();

	CView::OnLButtonDown(nFlags, point);
}


void CPainterView::OnLButtonUp(UINT nFlags, CPoint point)
{
	auto doc = GetDocument();
	if (doc == nullptr)
		return;

	if (_temp_line)
	{
		doc->AddShape(_temp_line);
		_temp_line.reset();
	} 
	else if (_temp_rect)
	{
		doc->AddShape(_temp_rect);
		_temp_rect.reset();
	}
	CView::OnLButtonUp(nFlags, point);
}


void CPainterView::OnMouseMove(UINT nFlags, CPoint point)
{
 	if ((nFlags & MK_LBUTTON) == MK_LBUTTON && _temp_line)
 	{
 		_temp_line->SetPoint2(Point(point.x, point.y));
 	}

	if ((nFlags & MK_LBUTTON) == MK_LBUTTON && _temp_rect)
	{
		_temp_rect->SetWidth(point.x - _temp_rect->GetX());
		_temp_rect->SetHeight(point.y - _temp_rect->GetY());
	}

	Invalidate(FALSE);
	UpdateWindow();

	CView::OnMouseMove(nFlags, point);
}

void CPainterView::OnButtonLine()
{
	_tool = ToolTypeLine;
}

void CPainterView::OnUpdateButtonLine(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(_tool == ToolTypeLine);
}

void CPainterView::OnButtonRectangle()
{
	_tool = ToolTypeRectangle;
}

void CPainterView::OnUpdateButtonRectangle(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(_tool == ToolTypeRectangle);
}

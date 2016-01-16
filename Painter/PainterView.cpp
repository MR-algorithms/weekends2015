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
#include "Tool.h"

#include "line.h"
#include "Rectangle.h"
#include "Ellipse.h"
#include "LineTool.h"
#include "RectangleTool.h"
#include "EllipseTool.h"
#include "MainFrm.h"
#include "PolygonTool.h"
#include "SelectTool.h"

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
	ON_COMMAND(ID_BUTTON_ELLIPSE, &CPainterView::OnButtonEllipse)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_ELLIPSE, &CPainterView::OnUpdateButtonEllipse)
	ON_COMMAND(ID_BUTTON_BORDER_COLOR, &CPainterView::OnButtonBorderColor)
	ON_COMMAND(ID_BUTTON_FILL_COLOR, &CPainterView::OnButtonFillColor)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_BUTTON_POLYGON, &CPainterView::OnButtonPolygon)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_POLYGON, &CPainterView::OnUpdateButtonPolygon)
	ON_COMMAND(ID_BUTTON_SELECT, &CPainterView::OnButtonSelect)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_SELECT, &CPainterView::OnUpdateButtonSelect)
	ON_COMMAND(ID_BUTTON_COMBINE, &CPainterView::OnButtonGroup)
	ON_COMMAND(ID_BUTTON_UNGROUP, &CPainterView::OnButtonUngroup)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_UNGROUP, &CPainterView::OnUpdateButtonUngroup)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_COMBINE, &CPainterView::OnUpdateButtonGroup)
	ON_COMMAND(ID_EDIT_CUT, &CPainterView::OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, &CPainterView::OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, &CPainterView::OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, &CPainterView::OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, &CPainterView::OnUpdateEditPaste)
END_MESSAGE_MAP()

// CPainterView construction/destruction

CPainterView::CPainterView() :
	_tool(ToolTypeLine)
{
	CTool::SetShapeUser(this);

	_tools.insert(make_pair(ToolTypeSelect, shared_ptr<CSelectTool>(new CSelectTool)));
	_tools.insert(make_pair(ToolTypeLine, shared_ptr<CLineTool>(new CLineTool)));
	_tools.insert(make_pair(ToolTypeRectangle, shared_ptr < CRectangleTool>(new CRectangleTool)));
	_tools.insert(make_pair(ToolTypeEllipse, shared_ptr < CEllipseTool>(new CEllipseTool)));
	_tools.insert(make_pair(ToolTypePolygon, shared_ptr<CPolygonTool>(new CPolygonTool)));
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

	auto shape = _tools[_tool]->GetShape();
	if (shape)
	{
		shape->Draw(graphics);
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
	_down_point = point;

	ASSERT(_tools[_tool]);
	_tools[_tool]->OnLButtonDown(nFlags, point);

	Invalidate(FALSE);
	UpdateWindow();

	CView::OnLButtonDown(nFlags, point);
}


void CPainterView::OnLButtonUp(UINT nFlags, CPoint point)
{
	ASSERT(_tools[_tool]);

	if (_tools[_tool]->OnLButtonUp(nFlags, point))
	{
		Invalidate(FALSE);
		UpdateWindow();
	}

	CView::OnLButtonUp(nFlags, point);
}


void CPainterView::OnMouseMove(UINT nFlags, CPoint point)
{
	ASSERT(_tools[_tool]);

	if (_tools[_tool]->OnMouseMove(nFlags, point))
	{
		Invalidate(FALSE);
		UpdateWindow();
	}

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


void CPainterView::OnButtonEllipse()
{
	_tool = ToolTypeEllipse;
}



void CPainterView::OnUpdateButtonEllipse(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(_tool == ToolTypeEllipse);
}


void CPainterView::OnButtonPolygon()
{
	_tool = ToolTypePolygon;
}


void CPainterView::OnUpdateButtonPolygon(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(_tool == ToolTypePolygon);
}

void CPainterView::OnButtonSelect()
{
	_tool = ToolTypeSelect;
}

void CPainterView::OnUpdateButtonSelect(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(_tool == ToolTypeSelect);
}

void CPainterView::OnButtonBorderColor()
{
	auto main_frame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(main_frame != nullptr);

	auto& ribbon = main_frame->GetRibbon();

	auto color_button = dynamic_cast<CMFCRibbonColorButton*>(ribbon.FindByID(ID_BUTTON_BORDER_COLOR));
	auto color = color_button->GetColor();

	_tools[_tool]->SetBorderColor(Gdiplus::Color(255, 
		GetRValue(color), GetGValue(color), GetBValue(color)));
}


void CPainterView::OnButtonFillColor()
{
	auto main_frame = dynamic_cast<CMainFrame*>(AfxGetMainWnd());
	ASSERT(main_frame != nullptr);

	auto& ribbon = main_frame->GetRibbon();

	auto color_button = dynamic_cast<CMFCRibbonColorButton*>(ribbon.FindByID(ID_BUTTON_FILL_COLOR));
	auto color = color_button->GetColor();

	_tools[_tool]->SetFillColor(Gdiplus::Color(255,
		GetRValue(color), GetGValue(color), GetBValue(color)));
}


void CPainterView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	ASSERT(_tools[_tool]);
	_tools[_tool]->OnLButtonDoubleClick(nFlags, point);

	CView::OnLButtonDblClk(nFlags, point);
}

bool CPainterView::AddShape(std::shared_ptr<CShape> shape)
{
	if (!shape)
		return false;

	auto doc = GetDocument();
	if (doc == nullptr)
		return false;

	doc->AddShape(shape);

	return true;
}

const std::vector<std::shared_ptr<CShape>> & CPainterView::Shapes() const
{
	auto doc = GetDocument();
	ASSERT(doc != nullptr);

	return doc->GetShapes();
}

void CPainterView::OnButtonGroup()
{
	auto doc = GetDocument();
	ASSERT(doc != nullptr);
	doc->Group();

	Invalidate(FALSE);
	UpdateWindow();
}

void CPainterView::OnUpdateButtonGroup(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}

void CPainterView::OnButtonUngroup()
{
	auto doc = GetDocument();
	ASSERT(doc != nullptr);

	doc->Ungroup();

	Invalidate(FALSE);
	UpdateWindow();
}


void CPainterView::OnUpdateButtonUngroup(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
}



void CPainterView::OnEditCut()
{
	auto doc = GetDocument();
	ASSERT(doc != nullptr);

	doc->Cut();

	Invalidate(FALSE);
	UpdateWindow();
	
	// TODO: Add your command handler code here
}


void CPainterView::OnEditCopy()
{
	auto doc = GetDocument();
	ASSERT(doc != nullptr);
	if (doc->CanCopy())
	{
		doc->Copy();
	}
}

void CPainterView::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetDocument()->CanCopy());
	// TODO: Add your command update UI handler code here
}

void CPainterView::OnEditPaste()
{
	auto doc = GetDocument();
	ASSERT(doc != nullptr);
	if (doc->CanPaste())
	{
		doc->Paste();
	}

	Invalidate(FALSE);
	UpdateWindow();
}

void CPainterView::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(GetDocument()->CanPaste());
	// TODO: Add your command update UI handler code here
}

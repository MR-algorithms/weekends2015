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

// PainterView.h : interface of the CPainterView class
//

#pragma once

#include <vector>
#include <memory>
#include <map>
#include "Tool.h"

enum ToolType
{
	ToolTypeSelect,
	ToolTypeLine,
	ToolTypeRectangle,
	ToolTypeEllipse,
	ToolTypePolygon,
};

class CTool;
class CLine;
class CRectangle;
class CEllipse;

class CPainterView : public CView, public IShapeUser
{
protected: // create from serialization only
	CPainterView();
	DECLARE_DYNCREATE(CPainterView)

// Attributes
public:
	CPainterDoc* GetDocument() const;

public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	void Draw(Gdiplus::Graphics& graphics);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	virtual bool AddShape(std::shared_ptr<CShape> shape);

// Implementation
public:
	virtual ~CPainterView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

private:
	std::map<ToolType, std::shared_ptr<CTool>> _tools;
	ToolType _tool;

	CPoint _down_point;

public:
	afx_msg void OnButtonLine();
	afx_msg void OnUpdateButtonLine(CCmdUI *pCmdUI);
	afx_msg void OnButtonRectangle();
	afx_msg void OnUpdateButtonRectangle(CCmdUI *pCmdUI);
	afx_msg void OnButtonEllipse();
	afx_msg void OnUpdateButtonEllipse(CCmdUI *pCmdUI);
	afx_msg void OnButtonBorderColor();
	afx_msg void OnButtonFillColor();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnButtonPolygon();
	afx_msg void OnUpdateButtonPolygon(CCmdUI *pCmdUI);
	afx_msg void OnButtonSelect();
	afx_msg void OnUpdateButtonSelect(CCmdUI *pCmdUI);

	virtual const std::vector<std::shared_ptr<CShape>> & Shapes() const override;
	afx_msg void OnButtonGroup();
	afx_msg void OnButtonUngroup();
	afx_msg void OnUpdateButtonUngroup(CCmdUI *pCmdUI);
	afx_msg void OnUpdateButtonGroup(CCmdUI *pCmdUI);
	afx_msg void OnEditCut();
	afx_msg void OnEditCopy();
	afx_msg void OnEditPaste();
	afx_msg void OnUpdateEditCopy(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditPaste(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // debug version in PainterView.cpp
inline CPainterDoc* CPainterView::GetDocument() const
   { return reinterpret_cast<CPainterDoc*>(m_pDocument); }
#endif


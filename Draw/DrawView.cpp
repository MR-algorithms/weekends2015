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

// DrawView.cpp : implementation of the CDrawView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Draw.h"
#endif

#include "DrawDoc.h"
#include "DrawView.h"

#include "MrSimulator.h"
#include "DataGenerator.h"
using namespace Gdiplus;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDrawView

IMPLEMENT_DYNCREATE(CDrawView, CView)

BEGIN_MESSAGE_MAP(CDrawView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDrawView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_CHECK_IMAGINARY, &CDrawView::OnCheckImaginary)
	ON_UPDATE_COMMAND_UI(ID_CHECK_IMAGINARY, &CDrawView::OnUpdateCheckImaginary)
	ON_COMMAND(ID_CHECK_MAGNITUDE, &CDrawView::OnCheckMagnitude)
	ON_UPDATE_COMMAND_UI(ID_CHECK_MAGNITUDE, &CDrawView::OnUpdateCheckMagnitude)
	ON_COMMAND(ID_CHECK_REAL, &CDrawView::OnCheckReal)
	ON_UPDATE_COMMAND_UI(ID_CHECK_REAL, &CDrawView::OnUpdateCheckReal)
END_MESSAGE_MAP()

// CDrawView construction/destruction

CDrawView::CDrawView() :
	_show_real(true),
	_show_imaginary(true),
	_show_magnitude(true)
{
}

CDrawView::~CDrawView()
{
}

BOOL CDrawView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CDrawView drawing

void CDrawView::OnDraw(CDC* pDC)
{
}

std::vector<double> CDrawView::GetReal(const std::vector<std::complex<double>>& source)
{
	std::vector<double> output(source.size());
	for (unsigned int i = 0; i < source.size(); ++i)
	{
		output[i] = source[i].real();
	}

	return output;
}

std::vector<double> CDrawView::GetImage(const std::vector<std::complex<double>>& source)
{
	std::vector<double> output(source.size());
	for (unsigned int i = 0; i < source.size(); ++i)
	{
		output[i] = source[i].imag();
	}

	return output;
}

std::vector<double> CDrawView::GetAmplitude(const std::vector<std::complex<double>>& source)
{
	std::vector<double> output(source.size());
	for (unsigned int i = 0; i < source.size(); ++i)
	{
		output[i] = sqrt(source[i].imag() * source[i].imag() + source[i].real() * source[i].real());
	}

	return output;
}

// CDrawView printing
void CDrawView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDrawView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDrawView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDrawView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CDrawView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDrawView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDrawView diagnostics

#ifdef _DEBUG
void CDrawView::AssertValid() const
{
	CView::AssertValid();
}

void CDrawView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDrawDoc* CDrawView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDrawDoc)));
	return (CDrawDoc*)m_pDocument;
}
#endif //_DEBUG


// CDrawView message handlers


int CDrawView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rc;
	if (_plot_window.Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rc, this, (UINT)-1) == -1)
		return -1;

	auto doc = GetDocument();
	if (doc == nullptr)
		return -1;

	auto time = doc->GetTimeSeries();
	auto m = doc->GetData();

	auto real_part = GetReal(m);
	auto imaginary_part = GetImage(m);
	auto amplitude = GetAmplitude(m);

	_plot_window.AddData(_T("Real"), time, real_part, Color::Red);

	_plot_window.AddData(_T("Imaginary"), time, imaginary_part, Color::Blue);
	_plot_window.AddData(_T("Magnitude"), time, amplitude, Color::Green);

	return 0;
}


void CDrawView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	if (nType != SIZE_RESTORED && nType != SIZE_MAXIMIZED)
		return;

	if (cx == 0 || cy == 0)
		return;

	CRect rect;
	GetClientRect(rect);

	_plot_window.MoveWindow(rect);
}


void CDrawView::OnCheckImaginary()
{
	_show_imaginary = !_show_imaginary;
	_plot_window.Show(_T("Imaginary"), _show_imaginary);
}


void CDrawView::OnUpdateCheckImaginary(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(_show_imaginary ? 1 : 0);
}


void CDrawView::OnCheckMagnitude()
{
	_show_magnitude = !_show_magnitude;
	_plot_window.Show(_T("Magnitude"), _show_magnitude);
}


void CDrawView::OnUpdateCheckMagnitude(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(_show_magnitude ? 1 : 0);
}


void CDrawView::OnCheckReal()
{
	_show_real = !_show_real;
	_plot_window.Show(_T("Real"), _show_real);
}


void CDrawView::OnUpdateCheckReal(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(_show_real ? 1 : 0);
}

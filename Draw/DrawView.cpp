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

#include <gdiplus.h>
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
END_MESSAGE_MAP()

// CDrawView construction/destruction

CDrawView::CDrawView()
{
	// TODO: add construction code here

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
	CDrawDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	Graphics my_graphics(pDC->m_hDC);
//	Draw1(my_graphics);
	Draw2(my_graphics);
}

void CDrawView::Draw1(Graphics& graphics)
{
	CRect client_rect;
	GetClientRect(client_rect);

	std::vector<double> time(1000);

	for (unsigned int i = 0; i < 1000; i++)
	{
		time[i] = 0.01 * i;
	}

	auto m = GenerateData(100, 2, 2, time);
	auto real_part = GetReal(m);
	auto image_part = GetImage(m);

	auto x_coodinates = CoordinateTransform(0, 10.0, 0, client_rect.Width(), time);
	auto y_coodinates = CoordinateTransform(-100.0, 100.0, client_rect.Height() / 2, 0, real_part);
	auto y_coodinate_imaginary = CoordinateTransform(-100.0, 100.0,
		client_rect.Height(), client_rect.Height() / 2, image_part);

	DrawLines(graphics, x_coodinates, y_coodinates);
	DrawLines(graphics, x_coodinates, y_coodinate_imaginary);
}

void CDrawView::Draw2(Graphics& graphics)
{
	CRect client_rect;
	GetClientRect(client_rect);

	// ×ö»­²¼
	Bitmap bitmap(client_rect.Width(), client_rect.Height(), &graphics);
	Gdiplus::Graphics buffer_graphics(&bitmap);
	SolidBrush BKbrush(Color::White);
	buffer_graphics.FillRectangle(&BKbrush, 0, 0, client_rect.Width(), client_rect.Height());

	std::vector<double> time(1000);

	for (unsigned int i = 0; i < 1000; i++)
	{
		time[i] = 0.01 * i;
	}

	auto m = GenerateData(100, 1, 2, time);
	auto real_part = GetReal(m);
	auto image_part = GetImage(m);

	auto x_coodinates = CoordinateTransform(-100, 100, 0, client_rect.Width(), real_part);
	auto y_coodinates = CoordinateTransform(-100.0, 100.0, client_rect.Height(), 0, image_part);

	DrawLines(buffer_graphics, x_coodinates, y_coodinates);

	graphics.DrawImage(&bitmap, client_rect.left, client_rect.top, client_rect.right, client_rect.bottom);
}

std::vector<std::complex<double>> CDrawView::GenerateData(double m0,
	double t2,
	double freq,
	const std::vector<double>& time)
{
	const double PI = 3.1415926535897932384626433832795;

	std::vector<std::complex<double>> output;
	output.resize(time.size());

	std::complex<double> I(0, 1);

	for (unsigned int i = 0; i < time.size(); ++i)
	{
		output[i] = m0 * exp(-time[i] / t2) * exp(I * 2.0 * PI * freq * time[i]);
	}

	return output;
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

std::vector<double> CDrawView::CoordinateTransform(double source_min,
	double source_max,
	double dest_min,
	double dest_max,
	const std::vector<double>& source)
{
	std::vector<double> output(source.size());

	double a = (dest_max - dest_min) / (source_max - source_min);
	double b = -source_min * a + dest_min;

	for (unsigned int i = 0; i < source.size(); ++i)
	{
		output[i] = a * source[i] + b;
	}

	return output;
}

void CDrawView::DrawLines(Gdiplus::Graphics& graphics,
	const std::vector<double>& x,
	const std::vector<double>& y)
{
	ASSERT(x.size() == y.size());

	Pen pen(Color::DarkOrange);

	for (unsigned int i = 0; i < x.size() - 1; ++i)
	{
		graphics.DrawLine(&pen, static_cast<float>(x[i]), y[i], x[i + 1], y[i + 1]);
	}
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

// PlotWnd.cpp : implementation file
//

#include "stdafx.h"
#include "Draw.h"
#include "PlotWnd.h"
#include "CoordinateTransformer.h"
#include <algorithm>
#include <Gdiplus.h>

using namespace std;
using namespace Gdiplus;

// CPlotWnd

IMPLEMENT_DYNAMIC(CPlotWnd, CWnd)

CPlotWnd::CPlotWnd()
{

}

CPlotWnd::~CPlotWnd()
{
}


BEGIN_MESSAGE_MAP(CPlotWnd, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CPlotWnd message handlers


std::vector<double> CPlotWnd::CoordinateTransform(double source_min, double source_max,
	double dest_min, double dest_max, const std::vector<double> & source)
{
	CCoordinateTransformer transformer(source_min, source_max, dest_min, dest_max);

	std::vector<double> output(source.size());
	for (unsigned int i = 0; i < source.size(); ++i)
	{
		output[i] = transformer.Transform(source[i]);
	}

	return output;
}

void CPlotWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	Graphics graphics(dc.m_hDC);

	CRect rect;
	GetClientRect(rect);

	Bitmap bitmap(rect.Width(), rect.Height(), &graphics);
	Gdiplus::Graphics buffer_graphics(&bitmap);
	SolidBrush BKbrush(Color::White);
	buffer_graphics.FillRectangle(&BKbrush, 0, 0, rect.Width(), rect.Height());

	auto x_range = GetXRange();
	auto y_range = GetYRange();

	for (auto iter = _data_series.begin(); iter != _data_series.end(); ++iter)
	{
		if (iter->second->IsVisible())
		{
			auto x_coodinates = CoordinateTransform(x_range.first, x_range.second, 0, rect.Width(), iter->second->GetX());
			auto y_coodinates = CoordinateTransform(y_range.first, y_range.second, rect.Height(), 0, iter->second->GetY());

			DrawLines(buffer_graphics, x_coodinates, y_coodinates, iter->second->GetColor());
		}
	}

	graphics.DrawImage(&bitmap, rect.left, rect.top, rect.right, rect.bottom);
}

pair<double, double> CPlotWnd::GetXRange()
{
	if (_data_series.empty())
		return make_pair(0.0, 0.0);


	auto iter = _data_series.begin();
	pair<double, double> min_max = iter->second->GetMinMaxX();
	++iter;
	for (; iter != _data_series.end(); ++iter)
	{
		pair<double, double> series_min_max = iter->second->GetMinMaxX();
		if (series_min_max.first < min_max.first)
		{
			min_max.first = series_min_max.first;
		}
		if (series_min_max.second > min_max.second)
		{
			min_max.second = series_min_max.second;
		}
	}

	return min_max;
}

pair<double, double> CPlotWnd::GetYRange()
{
	if (_data_series.empty())
		return make_pair(0.0, 0.0);


	auto iter = _data_series.begin();
	pair<double, double> min_max = iter->second->GetMinMaxY();
	++iter;
	for (; iter != _data_series.end(); ++iter)
	{
		pair<double, double> series_min_max = iter->second->GetMinMaxY();
		if (series_min_max.first < min_max.first)
		{
			min_max.first = series_min_max.first;
		}
		if (series_min_max.second > min_max.second)
		{
			min_max.second = series_min_max.second;
		}
	}

	return min_max;
}

void CPlotWnd::DrawLines(Gdiplus::Graphics& graphics,
	const std::vector<double>& x,
	const std::vector<double>& y,
	Color color)
{
	ASSERT(x.size() == y.size());

	Pen pen(color);

	for (unsigned int i = 0; i < x.size() - 1; ++i)
	{
		graphics.DrawLine(&pen, static_cast<float>(x[i]), static_cast<float>(y[i]), 
			static_cast<float>(x[i + 1]), static_cast<float>(y[i + 1]));
	}
}

bool CPlotWnd::AddData(const CString& name, 
	const std::vector<double>& x, 
	const std::vector<double>& y, 
	Color color)
{
	if (x.size() != y.size())
		return false;

	shared_ptr<CDataSeries> series(new CDataSeries());
	series->SetData(x, y);
	series->SetColor(color);

	_data_series.insert(make_pair(name, series));

	return true;
}

bool CPlotWnd::Show(const CString& name, bool show)
{
	auto iter = _data_series.find(name);
	if (iter == _data_series.end())
	{
		return false;
	}

	iter->second->Show(show);

	Invalidate(FALSE);
	UpdateWindow();

	return true;
}

CDataSeries::CDataSeries() :
	_visible(true),
	_color(Color::Black)
{

}


bool CDataSeries::SetData(const std::vector<double>& x, const std::vector<double>& y)
{
	if (x.size() != y.size())
		return false;

	_x = x;
	_y = y;

	return true;
}

void CDataSeries::SetColor(Gdiplus::Color color)
{
	_color = color;
}

Gdiplus::Color CDataSeries::GetColor() const
{
	return _color;
}

std::pair<double, double> CDataSeries::GetMinMaxX() const
{
	auto min_max_iter = minmax_element(_x.begin(), _x.end());
	pair<double, double> result;
	result.first = *min_max_iter.first;
	result.second = *min_max_iter.second;

	return result;
}

std::pair<double, double> CDataSeries::GetMinMaxY() const
{
	auto min_max_iter = minmax_element(_y.begin(), _y.end());
	pair<double, double> result;
	result.first = *min_max_iter.first;
	result.second = *min_max_iter.second;

	return result;
}

const std::vector<double>& CDataSeries::GetX() const
{
	return _x;
}

const std::vector<double>& CDataSeries::GetY() const
{
	return _y;
}

void CDataSeries::Show(bool show)
{
	_visible = show;
}

bool CDataSeries::IsVisible() const
{
	return _visible;
}

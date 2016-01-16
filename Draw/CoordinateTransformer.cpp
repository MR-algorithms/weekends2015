#include "stdafx.h"
#include "CoordinateTransformer.h"

CCoordinateTransformer::CCoordinateTransformer(double source_min,
	double source_max, 
	double dest_min, 
	double dest_max)
{
	_a = (dest_max - dest_min) / (source_max - source_min);
	_b = -source_min * _a + dest_min;
}

double CCoordinateTransformer::Transform(double source)
{
	return source * _a + _b;
}
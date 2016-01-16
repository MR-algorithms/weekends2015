#pragma once
#include <vector>

class CCoordinateTransformer
{
public:
	CCoordinateTransformer(double source_min, double source_max, double dest_min, double dest_max);
	double Transform(double source);

	double _a;
	double _b;
};


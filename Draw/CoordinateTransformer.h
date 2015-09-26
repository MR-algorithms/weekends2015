#pragma once

#include <vector>


class CCoordinateTransformer
{
public:
	CCoordinateTransformer();
	~CCoordinateTransformer();

	static std::vector<double> CoordinateTransform(double dest_min, double dest_max, const std::vector<double>& source);
};


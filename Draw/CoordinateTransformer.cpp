#include "stdafx.h"
#include "CoordinateTransformer.h"


CCoordinateTransformer::CCoordinateTransformer()
{
}


CCoordinateTransformer::~CCoordinateTransformer()
{
}

std::vector<double> CCoordinateTransformer::CoordinateTransform(double dest_min, 
	double dest_max,
	const std::vector<double>& source)
{
	std::vector<double> output(source.size());

	double source_max = source[0], source_min = source[0];
	for (int i = 1; i < source.size(); ++i)
	{
		if (source[i]>source_max)
		{
			source_max = source[i];
		}
		else if (source[i]<source_min)
		{
			source_min = source[i];
		}
	}

	double a = (dest_max - dest_min) / (source_max - source_min);
	double b = -source_min * a + dest_min;

	for (unsigned int i = 0; i < source.size(); ++i)
	{
		output[i] = a * source[i] + b;
	}

	return output;
}
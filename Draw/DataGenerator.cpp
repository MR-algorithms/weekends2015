#include "stdafx.h"
#include "DataGenerator.h"


CDataGenerator::CDataGenerator()
{
}


CDataGenerator::~CDataGenerator()
{
}

std::vector<double> CDataGenerator::GenerateEqualSpacedData(double x0, double delta_x, unsigned int count)
{
	std::vector<double> output(count);

	for (unsigned int i = 0; i < count; ++i)
	{
		output[i] = x0 + i * delta_x;
	}

	return output;
}

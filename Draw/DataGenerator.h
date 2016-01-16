#pragma once

#include <vector>

class CDataGenerator
{
public:
	CDataGenerator();
	virtual ~CDataGenerator();

	static std::vector<double> GenerateEqualSpacedData(double x0, double delta_x, unsigned int count);
};


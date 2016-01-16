#include "stdafx.h"
#include "MrSimulator.h"


CMrSimulator::CMrSimulator()
{
}


CMrSimulator::~CMrSimulator()
{
}

std::vector<std::complex<double>> CMrSimulator::GenerateData(
	const std::vector<double>& time)
{
	const double PI = 3.1415926535897932384626433832795;
	std::complex<double> I(0, 1);

	std::vector<std::complex<double>> output(time.size(), 0);

	for (auto component = _components.begin(); component != _components.end(); ++component)
	{
		for (unsigned int i = 0; i < time.size(); ++i)
		{
			output[i] += (*component)->Value(time[i]);
		}
	}

	return output;
}

bool CMrSimulator::AddExponentialComponent(double m0, double t2, double freq)
{
	if (m0 <= 0 || t2 <= 0)
	{
		return false;
	}

	std::shared_ptr<CExponentialComponent> component(new CExponentialComponent(m0, t2, freq));
	_components.push_back(component);

	return true;
}

bool CMrSimulator::AddGaussianComponent(double m0, double t2, double freq)
{
	if (m0 <= 0 || t2 <= 0)
	{
		return false;
	}

	std::shared_ptr<CGaussianComponent> component(new CGaussianComponent(m0, t2, freq));
	_components.push_back(component);

	return true;
}

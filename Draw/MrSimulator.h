#pragma once

#include <vector>
#include <complex>
#include <memory>

struct IComponent
{
	virtual std::complex<double> Value(double x) = 0;
};

class CExponentialComponent : public IComponent
{
public:
	CExponentialComponent(double i_m0, double i_t2, double i_freq) : m0(i_m0), t2(i_t2), freq(i_freq) {};

	const double PI = 3.1415926535897932384626433832795;

	virtual std::complex<double> Value(double x)
	{
		static std::complex<double> I(0, 1);
		return m0 * exp(-x / t2) * exp(I * 2.0 * PI * freq * x);
	}

	double m0;
	double t2;
	double freq;
};

class CGaussianComponent : public IComponent
{
public:
	CGaussianComponent(double i_m0, double i_t2, double i_freq) : m0(i_m0), t2(i_t2), freq(i_freq) {};

	const double PI = 3.1415926535897932384626433832795;

	virtual std::complex<double> Value(double x)
	{
		static std::complex<double> I(0, 1);
		return m0 * exp(-x * x / t2 / t2) * exp(I * 2.0 * PI * freq * x);
	}

	double m0;
	double t2;
	double freq;
};


class CMrSimulator
{
public:
	CMrSimulator();
	virtual ~CMrSimulator();

	bool AddExponentialComponent(double m0, double t2, double freq);
	bool AddGaussianComponent(double m0, double t2, double freq);

	std::vector<std::complex<double>> GenerateData(const std::vector<double>& time);

	std::vector<std::shared_ptr<IComponent>> _components;
};


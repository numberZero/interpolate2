#pragma once
#include <functional>
#include "surface.hxx"
#include "functional.hxx"

struct BilinearInterpolator: protected FunctionData
{
public:
	BilinearInterpolator() = default;
	BilinearInterpolator(BilinearInterpolator const &) = default;
	BilinearInterpolator(BilinearInterpolator &&) = default;
	BilinearInterpolator(FunctionData const &input);
	BilinearInterpolator(FunctionData &&input);

	BilinearInterpolator &operator= (BilinearInterpolator const &) = default;
	BilinearInterpolator &operator= (BilinearInterpolator &&) = default;

	double operator() (double x, double y) const;
};

void make_surface(Surface &output, FunctionData const &input);
FunctionData make_fdata(double r, int N, std::function<double(double, double)> f);

// Deprecated:
void make_surface(Surface &output, double r, int N, std::function<double(double, double)> f);
void make_interpolated(Surface &output, FunctionData const &input, int steps);

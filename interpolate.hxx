#pragma once
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

#include "interpolate.hxx"
#include <cmath>
#include <algorithm>

BilinearInterpolator::BilinearInterpolator(FunctionData const &input) :
	FunctionData(input)
{
}

BilinearInterpolator::BilinearInterpolator(FunctionData &&input) :
	FunctionData(std::move(input))
{
}

double BilinearInterpolator::operator() (double x, double y) const
{
	auto x2 = std::upper_bound(xx.begin(), xx.end() - 1, x);
	auto y2 = std::upper_bound(yy.begin(), yy.end() - 1, y);
	int i = x2 - xx.begin();
	int j = y2 - yy.begin();
	if (i > 0)
		--i;
	if (j > 0)
		--j;
	double dx = (x - xx[i]) / (xx[i + 1] - xx[i]);
	double dy = (y - yy[j]) / (yy[j + 1] - yy[j]);
	return dx * dy * get_f(i + 1, j + 1) +
		(1 - dx) * dy * get_f(i, j + 1) +
		dx * (1 - dy) * get_f(i + 1, j) +
		(1 - dx) * (1 - dy) * get_f(i, j);
}

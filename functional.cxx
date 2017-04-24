#include "functional.hxx"
#include <cmath>

FunctionData::FunctionData(std::vector<double> const &x, std::vector<double> const &y, double f(double, double))
{
	init(std::vector<double>(x), std::vector<double>(y), f);
}

FunctionData::FunctionData(std::vector<double> &&x, std::vector<double> &&y, double f(double, double))
{
	init(std::move(x), std::move(y), f);
}

FunctionData::FunctionData(double x1, double x2, double y1, double y2, int x_cuts, int y_cuts, double f(double, double))
{
	std::vector<double> xx = make_uniform_points(x1, x2, x_cuts);
	std::vector<double> yy = make_uniform_points(y1, y2, y_cuts);
	init(std::move(xx), std::move(yy), f);

}

void FunctionData::init(std::vector<double> &&x, std::vector<double> &&y, double f(double, double))
{
	xx = std::move(x);
	yy = std::move(y);
	ff.resize(size_x() * size_y());
	int n = 0;
	for (int j = 0; j < size_y(); ++j)
		for (int i = 0; i < size_x(); ++i)
			ff[n++] = f(xx[i], yy[j]); // n == size_x() * j + i
}

std::vector<double> make_uniform_points(double x1, double x2, int N_cuts)
{
	std::vector<double> points;
	points.resize(N_cuts + 1);
	double dx = (x2 - x1) / N_cuts;
	for (int k = 0; k <= N_cuts; ++k)
		points[k] = x1 + k * dx;
	return points;
}

std::vector<double> make_chebyshev_points(double x1, double x2, int N_cuts)
{
	std::vector<double> points;
	int n = N_cuts - 1;
	double shift = (x2 + x1) / 2;
	double scale = (x2 - x1) / 2;
	points.resize(N_cuts + 1);
	points[0] = x1;
	for (int k = 1; k <= n; ++k) {
		double t = ((n - k) + 0.5) / n * M_PI;
		points[k] = std::cos(t) * scale + shift;
	}
	points[N_cuts] = x2;
	return points;
}

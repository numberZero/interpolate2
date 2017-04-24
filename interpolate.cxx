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

void make_surface(Surface &output, FunctionData const &input)
{
	output.resize(input.size_x() - 1, input.size_y() - 1);
	for (int j = 0; j < input.size_y(); ++j)
		for (int i = 0; i < input.size_x(); ++i)
			output.get(i, j) = Vertex(input.get_x(i), input.get_f(i, j), input.get_y(j));
}

FunctionData make_fdata(double r, int N, std::function<double(double, double)> f)
{
	auto f_scaled = [r,&f] (double x, double y) { return f(x * r, y * r) / r;};
	return FunctionData(-1.0, 1.0, -1.0, 1.0, 2 * N, 2 * N, f_scaled);
}

void make_interpolated(Surface &output, FunctionData const &input, int steps)
{
	double d = 1.0 / steps;
	int cuts_i = input.size_x() - 1;
	int cuts_j = input.size_y() - 1;
	output.resize(steps * cuts_i, steps * cuts_j);
	for (int j = 0; j < cuts_j; ++j) {
		double y1 = input.get_y(j);
		double y2 = input.get_y(j + 1);
		for (int i = 0; i < cuts_i; ++i) {
			double x1 = input.get_x(i);
			double x2 = input.get_x(i + 1);
			Vertex aa(x1, input.get_f(i, j), y1);
			Vertex ab(x1, input.get_f(i, j + 1), y2);
			Vertex ba(x2, input.get_f(i + 1, j), y1);
			Vertex bb(x2, input.get_f(i + 1, j + 1), y2);
			for (int l = 0; l <= steps; ++l) {
				double dy = d * l;
				for (int k = 0; k <= steps; ++k) {
					double dx = d * k;
					Vertex &v = output.get(steps * i + k, steps * j + l);
					v = dx * dy * bb +
						(1 - dx) * dy * ab +
						dx * (1 - dy) * ba +
						(1 - dx) * (1 - dy) * aa;
				}
			}
		}
	}
}
void make_surface(Surface &output, double r, int N, std::function<double(double, double)> f)
{
	make_surface(output, make_fdata(r, N, f));
}

#include "interpolate.hxx"
#include <cmath>

void make_surface(Surface& output, FunctionData const &input)
{
	output.resize(input.size_x(), input.size_y());
	for (int j = 0; j < input.size_y(); ++j)
		for (int i = 0; i < input.size_x(); ++i)
			output.get(i, j) = Vertex(input.get_x(i), input.get_f(i, j), input.get_y(j));
}

void make_surface(Surface& output, double r, int N, double f(double, double))
{
	output.resize(2 * N, 2 * N);
	double d = r / N;
	for (int i = 0; i < 2 * N + 1; ++i) {
		double u = d * (i - N);
		for (int j = 0; j < 2 * N + 1; ++j) {
			double v = d * (j - N);
			double w = f(u, v);
			output.get(i, j) = Vertex(u / r, w / r, v / r);
		}
	}
}

void make_interpolated(Surface& output, const Surface& input, int steps)
{
	double d = 1.0 / steps;
	int cuts_i = input.points_i - 1;
	int cuts_j = input.points_j - 1;
	output.resize(steps * cuts_i, steps * cuts_j);
	for (int j = 0; j < cuts_j; ++j)
		for (int i = 0; i < cuts_i; ++i) {
			Vertex const &aa = input.get(i, j);
			Vertex const &ab = input.get(i, j + 1);
			Vertex const &ba = input.get(i + 1, j);
			Vertex const &bb = input.get(i + 1, j + 1);
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

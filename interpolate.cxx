#include "interpolate.hxx"
#include <cmath>

void make_surface(Surface& output, double r, int N, double f(double, double))
{
	output.resize(2 * N);
	double d = r / N;
	for (int i = 0; i < output.side_points; ++i) {
		double u = d * (i - N);
		for (int j = 0; j < output.side_points; ++j) {
			double v = d * (j - N);
			double w = f(u, v);
			output.get(i, j) = Vertex(u / r, w / r, v / r);
		}
	}
}

void make_interpolated(Surface& output, const Surface& input, int steps)
{
	double d = 1.0 / steps;
	int src_cuts = input.side_points - 1;
	int dst_cuts = steps * src_cuts;
	output.resize(dst_cuts);
	for (int j = 0; j < src_cuts; ++j)
		for (int i = 0; i < src_cuts; ++i) {
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

#include "surface.hxx"

void Surface::create(FunctionData const &input)
{
	resize(input.size_x() - 1, input.size_y() - 1);
	for (int j = 0; j < input.size_y(); ++j)
		for (int i = 0; i < input.size_x(); ++i)
			get(i, j) = Vertex(input.get_x(i), input.get_f(i, j), input.get_y(j));
}

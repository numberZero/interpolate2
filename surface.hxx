#pragma once
#include <vector>
#include <GL/gl.h>

struct Vertex
{
	GLfloat x = 0, y = 0, z = 0;

	Vertex() = default;
	Vertex(Vertex const &) = default;
	Vertex(double u, double v, double w) : x(u), y(v), z(w) {}
};

struct Surface
{
	std::vector<Vertex> data;
	int size;
	int nn;
	void resize(int N);
	Vertex &get(int i, int j);
	GLfloat *vx(int i, int j);
};

inline void Surface::resize(int N)
{
	size = N;
	nn = 2 * N + 1;
	data = std::vector<Vertex>(nn * nn);
}

inline Vertex &Surface::get(int i, int j)
{
	return data[nn * j + i];
}

inline GLfloat *Surface::vx(int i, int j)
{
	return (GLfloat *)&get(i, j);
}

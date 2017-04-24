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
	int side_points;
	void resize(int size);
	Vertex &get(int i, int j);
	GLfloat *vx(int i, int j);
};

inline void Surface::resize(int size)
{
	side_points = size + 1;
	data = std::vector<Vertex>(side_points * side_points);
}

inline Vertex &Surface::get(int i, int j)
{
	return data[side_points * j + i];
}

inline GLfloat *Surface::vx(int i, int j)
{
	return (GLfloat *)&get(i, j);
}

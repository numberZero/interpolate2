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
	Vertex const &get(int i, int j) const;
	Vertex &get(int i, int j);
	GLfloat const *vx(int i, int j) const;
};

inline Vertex operator* (double m, Vertex const &v)
{
	return { m * v.x, m * v.y, m * v.z };
}

inline Vertex operator+ (Vertex const &u, Vertex const &v)
{
	return { u.x + v.x, u.y + v.y, u.z + v.z };
}

inline void Surface::resize(int size)
{
	side_points = size + 1;
	data = std::vector<Vertex>(side_points * side_points);
}

inline Vertex const &Surface::get(int i, int j) const
{
	return data[side_points * j + i];
}

inline Vertex &Surface::get(int i, int j)
{
	return data[side_points * j + i];
}

inline GLfloat const *Surface::vx(int i, int j) const
{
	return (GLfloat const *)&get(i, j);
}

#pragma once
#include <vector>
#include <GL/gl.h>
#include "functional.hxx"

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
	int points_i;
	int points_j;
	void create(FunctionData const &input);
	void resize(int cuts_i, int cuts_j);
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

inline void Surface::resize(int cuts_i, int cuts_j)
{
	points_i = cuts_i + 1;
	points_j = cuts_j + 1;
	data = std::vector<Vertex>(points_i * points_j);
}

inline Vertex const &Surface::get(int i, int j) const
{
	return data[points_i * j + i];
}

inline Vertex &Surface::get(int i, int j)
{
	return data[points_i * j + i];
}

inline GLfloat const *Surface::vx(int i, int j) const
{
	return (GLfloat const *)&get(i, j);
}

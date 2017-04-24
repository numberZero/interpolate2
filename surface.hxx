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
private:
	std::vector<Vertex> data;
	int points_i;
	int points_j;

	void resize(int cuts_i, int cuts_j);
	Vertex const &get(int i, int j) const;
	Vertex &get(int i, int j);
	GLfloat const *vx(int i, int j) const;

public:
	void create(FunctionData const &input);
	void drawP();
	void drawG();
	void drawQ();
	void draw(double r, double g, double b, double a);
};

inline Vertex operator* (double m, Vertex const &v)
{
	return { m * v.x, m * v.y, m * v.z };
}

inline Vertex operator+ (Vertex const &u, Vertex const &v)
{
	return { u.x + v.x, u.y + v.y, u.z + v.z };
}

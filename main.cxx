#include <cmath>
#include <iostream>
#include <vector>
#include <GL/gl.h>
#include <SDL.h>
#include "interpolate.hxx"
#include "surface.hxx"

SDL_Window *win;
SDL_GLContext ctx;
double t;
int w = 800, h = 600;
double const r = 2.5;

void cv(int x, int y,int z)
{
	glColor3f(x, y, z);
	glVertex3f(x - 0.5, y - 0.5, z - 0.5);
}

void cube()
{
	glPushMatrix();
	glRotatef(-30.0, 1.0, 0.0, 0.0);
	glRotatef(45.0, 0.0, 0.0, 1.0);
	glRotatef(90.0 * t, 1.0, 1.0, 1.0);
	glBegin(GL_QUADS);
	cv(0, 0, 0);
	cv(1, 0, 0);
	cv(1, 1, 0);
	cv(0, 1, 0);

	cv(0, 0,1);
	cv(1, 0,1);
	cv(1, 1,1);
	cv(0, 1,1);

	cv(0, 0, 0);
	cv(1, 0, 0);
	cv(1, 0, 1);
	cv(0, 0, 1);

	cv(0,1, 0);
	cv(1,1, 0);
	cv(1,1, 1);
	cv(0,1, 1);

	cv(0, 0, 0);
	cv(0, 1, 0);
	cv(0, 1, 1);
	cv(0, 0, 1);

	cv(1, 0, 0);
	cv(1, 1, 0);
	cv(1, 1, 1);
	cv(1, 0, 1);
	glEnd();
	glPopMatrix();
}

double sqr(double x)
{
	return x * x;
}

double fn(double u, double v)
{
	return std::exp(-0.5 * (sqr(u) + sqr(v) + sqr(2.0 * std::cos(1.0 + 2.0 * u * u - v * v * v))));
}

class Graph: public Surface
{
public:
	void drawP();
	void drawG();
	void drawQ();
};

void Graph::drawP()
{
	glBegin(GL_POINTS);
	for (int j = 0; j < points_j; ++j)
		for (int i = 0; i < points_i; ++i)
			glVertex3fv(vx(i, j));
	glEnd();
}

void Graph::drawG()
{
	for (int i = 0; i < points_i; ++i) {
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < points_j; ++j) {
			glVertex3fv(vx(i, j));
		}
		glEnd();
	}
	for (int j = 0; j < points_j; ++j) {
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < points_i; ++i) {
			glVertex3fv(vx(i, j));
		}
		glEnd();
	}
}

void Graph::drawQ()
{
	glBegin(GL_QUADS);
	for (int j = 0; j < points_j - 1; ++j) {
		for (int i = 0; i < points_i - 1; ++i) {
			glVertex3fv(vx(i, j));
			glVertex3fv(vx(i + 1, j));
			glVertex3fv(vx(i + 1, j + 1));
			glVertex3fv(vx(i, j + 1));
		}
	}
	glEnd();
}

Graph b, g;

void graph()
{
	glPushMatrix();
	glScalef(1.0 / r, 1.0 / r, 1.0 / r);
	glRotatef(30.0, 1.0, 0.0, 0.0);
	glRotatef(30.0 * t, 0.0, 1.0, 0.0);
	glColor4f(0.0, 0.0, 0.4, 0.01);
	g.drawQ();
	glTranslatef(0.0, 1e-3, 0.0);
	glColor4f(0.0, 1.0, 0.0, 0.3);
	g.drawG();
	glTranslatef(0.0, 1e-3, 0.0);
	glColor4f(0.0, 1.0, 0.0, 0.6);
	b.drawG();
	glColor4f(1.0, 0.0, 0.0, 1.0);
	b.drawP();
	glPopMatrix();
}

void step()
{
	t = 1e-3 * SDL_GetTicks();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double a, b;
	if (w < h) {
		a = 1.0;
		b = 1.0 * h / w;
	} else {
		a = 1.0 * w / h;
		b = 1.0;
	}
	glOrtho(-a, a, -b, b, -2.0, 2.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glDisable(GL_POLYGON_SMOOTH);

	glPointSize(2.5);
	glLineWidth(1.5);
	glClearColor(0.05, 0.00, 0.25, 1.00);
	glClearDepth(100.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// 	cube();
	graph();

	SDL_GL_SwapWindow(win);
}

bool processEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT)
			return false;
		if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
			SDL_GetWindowSize(win, &w, &h);
			glViewport(0, 0, w, h);
		}
	}
	return true;
}

void init_graph()
{
	FunctionData fd = make_fdata(r, 10, fn);
	BilinearInterpolator bil(fd);
	FunctionData fd2 = make_fdata(r, 50, bil);
	make_surface(b, fd);
	make_surface(g, fd2);
}

int main(int argc, char **argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	win = SDL_CreateWindow(
		"Magic Program",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		w,
		h,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
	);
	ctx = SDL_GL_CreateContext(win);
	SDL_GL_MakeCurrent(win, ctx);
	init_graph();
	for (;;) {
		if (!processEvents())
			break;
		step();
	}
	SDL_Quit();
	return 0;
}

#include <cmath>
#include <iostream>
#include <vector>
#include <GL/gl.h>
#include <SDL.h>

SDL_Window *win;
SDL_GLContext ctx;
double t;
int w = 800, h = 600;

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
	return std::exp(-0.5 * (sqr(u) + sqr(v) + sqr(std::sin(2.0 * (u * u + v * v)))));
}

struct Vertex
{
	GLfloat x = 0, y = 0, z = 0;

	Vertex() = default;
	Vertex(Vertex const &) = default;
	Vertex(double u, double v, double w) : x(u), y(v), z(w) {}
};

class Graph
{
private:
	std::vector<Vertex> data;
	int size;
	int nn;
	Vertex &get(int i, int j);
	GLfloat *vx(int i, int j);

public:
	void fill(double r, int N, double f(double, double));
	void drawG();
	void drawQ();
};

Vertex &Graph::get(int i, int j)
{
	return data[nn * j + i];
}

GLfloat *Graph::vx(int i, int j)
{
	return (GLfloat *)&get(i, j);
}

void Graph::fill(double r, int N, double f(double, double))
{
	size = N;
	nn = 2 * N + 1;
	data = std::vector<Vertex>(nn * nn);
	double d = r / N;
	for (int i = 0; i < nn; ++i) {
		double u = d * (i - N);
		for (int j = 0; j < nn; ++j) {
			double v = d * (j - N);
			double w = fn(u, v);
			get(i, j) = Vertex(u/r, w/r, v/r);
		}
	}
}

void Graph::drawG()
{
	for (int i = 0; i < nn; ++i) {
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < nn; ++j) {
			glVertex3fv(vx(i, j));
		}
		glEnd();
	}
	for (int j = 0; j < nn; ++j) {
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < nn; ++i) {
			glVertex3fv(vx(i, j));
		}
		glEnd();
	}
}

void Graph::drawQ()
{
	glBegin(GL_QUADS);
	for (int i = 0; i < 2 * size; ++i) {
		for (int j = 0; j < 2 * size; ++j) {
			glVertex3fv(vx(i, j));
			glVertex3fv(vx(i + 1, j));
			glVertex3fv(vx(i + 1, j + 1));
			glVertex3fv(vx(i, j + 1));
		}
	}
	glEnd();
}

Graph g;

void graph()
{
	glPushMatrix();
	glRotatef(30.0, 1.0, 0.0, 0.0);
	glRotatef(90.0 * t, 0.0, 1.0, 0.0);
	glColor4f(0.2, 0.2, 0.2, 1.0);
	g.drawQ();
	glTranslatef(0.0, 1e-3, 0.0);
	glColor4f(0.0, 1.0, 0.0, 0.7);
	g.drawG();
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
	glEnable(GL_LINE_SMOOTH);
	glDisable(GL_POLYGON_SMOOTH);

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
	g.fill(2.5, 40, fn);
	for (;;) {
		if (!processEvents())
			break;
		step();
	}
	SDL_Quit();
	return 0;
}

#include <cmath>
#include <iostream>
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
/*
Vertex operator+ (Vertex const &a, Vertex const &b)
{
	return {a.x + b.x, a.y + b.y, a.z + b.z};
}
*/
long const N = 40;
Vertex points[2 * N + 1][2 * N + 1];
Vertex points2[2 * N + 1][2 * N + 1];

void initPoints()
{
	double s = 2.5;
	double d = s / N;
	for (int i = -N; i <= N; ++i) {
		double u = d * i;
		for (int j = -N; j <= N; ++j) {
			double v = d * j;
			double w = fn(u, v);
			points[N + i][N + j] = Vertex(u/s, w/s, v/s);
			points2[N + i][N + j] = Vertex(u/s, w/s + 1e-3, v/s);
		}
	}
}

void graph()
{
	glPushMatrix();
	glRotatef(30.0, 1.0, 0.0, 0.0);
	glRotatef(90.0 * t, 0.0, 1.0, 0.0);
	glColor4f(0.2, 0.2, 0.2, 1.0);
	glBegin(GL_QUADS);
	for (int i = 0; i < 2 * N; ++i) {
		for (int j = 0; j < 2 * N; ++j) {
			glVertex3fv((GLfloat*)&points[i][j]);
			glVertex3fv((GLfloat*)&points[i+1][j]);
			glVertex3fv((GLfloat*)&points[i+1][j+1]);
			glVertex3fv((GLfloat*)&points[i][j+1]);
		}
	}
	glEnd();
	glColor4f(0.0, 1.0, 0.0, 0.7);
	for (int i = 0; i <= 2 * N; ++i) {
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j <= 2 * N; ++j) {
			glVertex3fv((GLfloat*)&points2[i][j]);
		}
		glEnd();
	}
	for (int j = 0; j <= 2 * N; ++j) {
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i <= 2 * N; ++i) {
			glVertex3fv((GLfloat*)&points2[i][j]);
		}
		glEnd();
	}
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
	initPoints();
	for (;;) {
		if (!processEvents())
			break;
		step();
	}
	SDL_Quit();
	return 0;
}

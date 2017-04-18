#include <cmath>
#include <iostream>
#include <GL/gl.h>
#include <SDL.h>

SDL_Window *win;
SDL_GLContext ctx;
double t;

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

double fn(double u, double v)
{
// 	return std::exp(-0.5 * (u * u + v * v));
	return std::sin(2.0 * (u * u + v * v)) * std::exp(-0.5 * (u * u + v * v));
}

void graph()
{
	long N = 20;
	double s = 3.0;
	double d = s / N;
	glPushMatrix();
	glScalef(1.0 / s, 1.0 / s, 1.0 / s);
	glRotatef(30.0, 1.0, 0.0, 0.0);
	glRotatef(90.0 * t, 0.0, 1.0, 0.0);
	for (int i = -N; i <= N; ++i) {
		double u = d * i;
		glBegin(GL_LINE_STRIP);
		for (int j = -N; j <= N; ++j) {
			double v = d * j;
			double w = fn(u, v);
			glVertex3f(u, w, v);
		}
		glEnd();
	}
	for (int j = -N; j <= N; ++j) {
		double v = d * j;
		glBegin(GL_LINE_STRIP);
		for (int i = -N; i <= N; ++i) {
			double u = d * i;
			double w = fn(u, v);
			glVertex3f(u, w, v);
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
	glOrtho(-8.0 / 6.0, 8.0 / 6.0, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);

	glClearColor(0.05, 0.00, 0.25, 1.00);
	glClearDepth(100.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// 	cube();

	glColor4f(0.0, 1.0, 0.0, 0.8);
	graph();

	SDL_GL_SwapWindow(win);
}

bool processEvents()
{
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT)
			return false;
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
		800,
		600,
		SDL_WINDOW_OPENGL
	);
	ctx = SDL_GL_CreateContext(win);
	SDL_GL_MakeCurrent(win, ctx);
	for (;;) {
		if (!processEvents())
			break;
		step();
	}
	SDL_Quit();
	return 0;
}

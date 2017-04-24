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
int N = 20;
double const r = 2.5;
double const dangle = 5.0;
double const dscale = 1.15;
double scale = 1.0 / r;
double angle;
bool rotating = true;
Surface sbase;
Surface sfunc;
Surface sinterp;
Surface serr;

enum class Mode
{
	Original = 0,
	Interpolated = 1,
	Error = 2
} mode = Mode::Interpolated;

double sqr(double x)
{
	return x * x;
}

double fn(double u, double v)
{
	return std::exp(-0.5 * (sqr(u) + sqr(v) + sqr(2.0 * std::cos(1.0 + 2.0 * u * u - v * v * v))));
}

void init_graphs(int N = 20, int steps = 5);

void graph()
{
	glPushMatrix();
	glScalef(scale, scale, scale);
	glRotatef(30.0, 1.0, 0.0, 0.0);
	glRotatef(angle, 0.0, 1.0, 0.0);

	switch(mode) {
		case Mode::Original:
			sfunc.draw(0.0, 1.0, 1.0, 0.3);
			break;

		case Mode::Interpolated:
			sinterp.draw(0.0, 1.0, 0.0, 0.3);

			glTranslatef(0.0, 2.0e-3, 0.0);
			glColor4f(0.0, 1.0, 0.0, 0.6);
			sbase.drawG();
			glColor4f(1.0, 0.0, 0.0, 1.0);
			sbase.drawP();
			break;

		case Mode::Error:
			serr.draw(1.0, 1.0, 0.0, 0.6);
			break;
	}
	glPopMatrix();
}

void step()
{
	static double t_prev = 0.0;
	t = 1e-3 * SDL_GetTicks();
	double dt = t - t_prev;
	if (rotating)
		angle += 30.0 * dt;
	t_prev = t;

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
			continue;
		}
		if (e.type == SDL_KEYDOWN) {
			switch (e.key.keysym.scancode) {
				case SDL_SCANCODE_1:
					mode = Mode::Original;
					break;
				case SDL_SCANCODE_2:
					mode = Mode::Interpolated;
					break;
				case SDL_SCANCODE_3:
					mode = Mode::Error;
					break;
				case SDL_SCANCODE_M:
					mode = Mode((int(mode) + 1) % 3);
					break;
				case SDL_SCANCODE_R:
					rotating = !rotating;
					break;
				case SDL_SCANCODE_KP_PLUS:
					scale *= dscale;
					break;
				case SDL_SCANCODE_KP_MINUS:
					scale /= dscale;
					break;
				case SDL_SCANCODE_KP_MULTIPLY:
					N *= 2;
					init_graphs(N);
					break;
				case SDL_SCANCODE_KP_DIVIDE:
					N /= 2;
					init_graphs(N);
					break;
				case SDL_SCANCODE_KP_4:
					angle -= dangle;
					break;
				case SDL_SCANCODE_KP_6:
					angle += dangle;
					break;
				case SDL_SCANCODE_ESCAPE:
					return false;
				default:
					break;
			}
		}
	}
	return true;
}

void init_graphs(int N, int steps)
{
	int M = steps * N;
	FunctionData fd(-r, r, -r, r, N, N, fn);
	BilinearInterpolator bil(fd);
	auto err = [&](double x, double y) { return std::abs(bil(x, y) - fn(x, y)); };
	FunctionData fd1(-r, r, -r, r, M, M, fn);
	FunctionData fd2(-r, r, -r, r, M, M, bil);
	FunctionData fde(-r, r, -r, r, M, M, err);
	sbase.create(fd);
	sfunc.create(fd1);
	sinterp.create(fd2);
	serr.create(fde);
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
	init_graphs(N);
	for (;;) {
		if (!processEvents())
			break;
		step();
	}
	SDL_Quit();
	return 0;
}

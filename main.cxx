#include "main.hxx"
#include <cmath>
#include <iostream>
#include <vector>
#include <SDL.h>
#include "interpolate.hxx"
#include "surface.hxx"
#include "user.hxx"
#include "visual.hxx"

int N = 20;
double const r = 2.5;

double const dangle = 5.0;
double const dscale = 1.15;
bool rotating = true;

double sqr(double x)
{
	return x * x;
}

double fn(double u, double v)
{
	return std::exp(-0.5 * (sqr(u) + sqr(v) + sqr(2.0 * std::cos(1.0 + 2.0 * u * u - v * v * v))));
}

void init_graphs(int N = 20, int steps = 5)
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

void pre_step(double, double dt)
{
	if (rotating)
		angle += 30.0 * dt;
}

void handle_key(int code)
{
	switch (code) {
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
			throw ExitException();
		default:
			break;
	}
}

int main()
{
	scale = 1.0 / r;
	user_init();
	init_graphs(N);
	user_mainloop();
	user_close();
	return 0;
}

#include "main.hxx"
#include <cmath>
#include <ctime>
#include <iostream>
#include <random>
#include <vector>
#include <GL/glut.h>
#include <SDL.h>
#include "functional.hxx"
#include "interpolate.hxx"
#include "surface.hxx"
#include "user.hxx"
#include "visual.hxx"

int N = 20;
double range = 1.0;
double error = 0.0;
int const steps = 5;
double const r = 2.5;

double const dangle = 5.0;
double const dscale = 1.15;
bool rotating = true;

std::ranlux24 gen;

double sqr(double x)
{
	return x * x;
}

double fn(double u, double v)
{
	return std::exp(-0.5 * (sqr(u) + sqr(v) + sqr(2.0 * std::cos(1.0 + 2.0 * u * u - v * v * v))));
}

FunctionData fd;
FunctionData fd1;

void make_graphs()
{
	int M = steps * N;
	BilinearInterpolator bil(fd);
	auto err = [&](double x, double y) { return std::abs(bil(x, y) - fn(x, y)); };
	FunctionData fd2(-r, r, -r, r, M, M, bil);
	FunctionData fde(-r, r, -r, r, M, M, err);
	error = fde.range();
	sbase.create(fd);
	sfunc.create(fd1);
	sinterp.create(fd2);
	serr.create(fde);
}

void init_graphs()
{
	int M = steps * N;
	fd = FunctionData(-r, r, -r, r, N, N, fn);
	fd1 = FunctionData(-r, r, -r, r, M, M, fn);
	range = fd.range();
	scalev = 0.5 / range;
	make_graphs();
}

void damage()
{
	std::uniform_int_distribution<> dx(0, fd.size_x() - 1);
	std::uniform_int_distribution<> dy(0, fd.size_y() - 1);
	std::normal_distribution<> N(0.0, 0.1 * range);
	int i = dx(gen);
	int j = dy(gen);
	double z = fd.get_f(i, j);
	z += N(gen);
	fd.set_f(i, j, z);
	fd1.set_f(i * steps, j * steps, z);
	make_graphs();
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
		case SDL_SCANCODE_D:
			damage();
			break;
		case SDL_SCANCODE_E:
			init_graphs();
			break;
		case SDL_SCANCODE_KP_PLUS:
			scaleh *= dscale;
			scalev *= dscale;
			break;
		case SDL_SCANCODE_KP_MINUS:
			scaleh /= dscale;
			scalev /= dscale;
			break;
		case SDL_SCANCODE_KP_MULTIPLY:
			N *= 2;
			init_graphs();
			break;
		case SDL_SCANCODE_KP_DIVIDE:
			N /= 2;
			init_graphs();
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

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	gen.seed(std::time(nullptr));
	scaleh = 1.0 / r;
	user_init();
	init_graphs();
	user_mainloop();
	user_close();
	return 0;
}

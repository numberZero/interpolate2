#pragma once
#include "surface.hxx"
#include "functional.hxx"

void make_surface(Surface& output, FunctionData const &input);
void make_interpolated(Surface &output, Surface const &input, int steps);

void make_surface(Surface& output, double r, int N, double f(double, double));
void make_interpolated(Surface &output, Surface const &input, int steps);

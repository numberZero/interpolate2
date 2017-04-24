#pragma once
#include <functional>
#include "surface.hxx"
#include "functional.hxx"

void make_surface(Surface& output, FunctionData const &input);
void make_surface(Surface& output, double r, int N, std::function<double(double, double)> f);
void make_interpolated(Surface &output, Surface const &input, int steps);

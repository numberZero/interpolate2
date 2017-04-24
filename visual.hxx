#pragma once

enum class Mode
{
	Original = 0,
	Interpolated = 1,
	Error = 2
};

void visual_begin(int window_width, int window_height);
void visual_end();
void visual_draw();

extern double scale;
extern double angle;
extern Mode mode;

extern class Surface sbase;
extern class Surface sfunc;
extern class Surface sinterp;
extern class Surface serr;

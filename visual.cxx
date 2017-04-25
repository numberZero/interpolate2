#include "visual.hxx"
#include <GL/gl.h>
#include "main.hxx"
#include "surface.hxx"
#include "text.hxx"

double scaleh;
double scalev;
double angle;

Mode mode = Mode::Interpolated;

Surface sbase;
Surface sfunc;
Surface sinterp;
Surface serr;
double a, b;

void visual_begin(int w, int h)
{
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
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
}

void visual_end()
{
	double x = 0.04 - a;
	double y = b;
	double s = 0.06;
	double h = 0.08;
	double t = 1.0;
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glDisable(GL_DEPTH_TEST);
	vglTextOutF(x, y -= h, s, t, "Value range: %.3f", range);
	vglTextOutF(x, y -= h, s, t, "Max. error: %.3g (%.1f%%)", error, 100.0 * error / range);
}

void visual_draw()
{
	glPushMatrix();
	glRotatef(30.0, 1.0, 0.0, 0.0);
	glRotatef(angle, 0.0, 1.0, 0.0);
	glScalef(scaleh, scalev, scaleh);

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

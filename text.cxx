#include "text.hxx"
#ifdef HAVE_GLUT
#include <cstdarg>
#include <cstdio>
#include <GL/gl.h>
#include <GL/glut.h>
#endif

void vglTextOut(double x, double y, double size, double thickness, char const *text)
{
#ifdef HAVE_GLUT
	glLineWidth(thickness);
	glPushMatrix();
	glTranslated(x, y, 0.0);
	glScaled(size/150.0, size/150.0, 0.0);
	for(; *text; ++text) {
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *text);
		glTranslated(8.0, 0.0, 0.0);
	}
	glPopMatrix();
#endif
}

void vglTextOutF(double x, double y, double size, double thickness, char const *text, ...)
{
#ifdef HAVE_GLUT
	static int const BUF_SIZE = 1024;
	char buf[BUF_SIZE];
	va_list list;
	va_start(list, text);
	vsnprintf(buf, BUF_SIZE, text, list);
	va_end(list);
	vglTextOut(x, y, size, thickness, buf);
#endif
}

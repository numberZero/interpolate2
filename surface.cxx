#include "surface.hxx"

inline void Surface::resize(int cuts_i, int cuts_j)
{
	points_i = cuts_i + 1;
	points_j = cuts_j + 1;
	data = std::vector<Vertex>(points_i * points_j);
}

inline Vertex const &Surface::get(int i, int j) const
{
	return data[points_i * j + i];
}

inline Vertex &Surface::get(int i, int j)
{
	return data[points_i * j + i];
}

inline GLfloat const *Surface::vx(int i, int j) const
{
	return (GLfloat const *)&get(i, j);
}

void Surface::create(FunctionData const &input)
{
	resize(input.size_x() - 1, input.size_y() - 1);
	for (int j = 0; j < input.size_y(); ++j)
		for (int i = 0; i < input.size_x(); ++i)
			get(i, j) = Vertex(input.get_x(i), input.get_f(i, j), input.get_y(j));
}

void Surface::drawP()
{
	glBegin(GL_POINTS);
	for (int j = 0; j < points_j; ++j)
		for (int i = 0; i < points_i; ++i)
			glVertex3fv(vx(i, j));
	glEnd();
}

void Surface::drawG()
{
	for (int i = 0; i < points_i; ++i) {
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < points_j; ++j) {
			glVertex3fv(vx(i, j));
		}
		glEnd();
	}
	for (int j = 0; j < points_j; ++j) {
		glBegin(GL_LINE_STRIP);
		for (int i = 0; i < points_i; ++i) {
			glVertex3fv(vx(i, j));
		}
		glEnd();
	}
}

void Surface::drawQ()
{
	glBegin(GL_QUADS);
	for (int j = 0; j < points_j - 1; ++j) {
		for (int i = 0; i < points_i - 1; ++i) {
			glVertex3fv(vx(i, j));
			glVertex3fv(vx(i + 1, j));
			glVertex3fv(vx(i + 1, j + 1));
			glVertex3fv(vx(i, j + 1));
		}
	}
	glEnd();
}

void Surface::draw(double r, double g, double b, double a)
{
	glColor4f(0.0, 0.0, 0.0, 0.01);
	drawQ();
	glTranslatef(0.0, 1.0e-3, 0.0);
	glColor4f(r, g, b, a);
	drawG();
	glTranslatef(0.0, -1.0e-3, 0.0);
}

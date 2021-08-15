#include <GL/gl.h>

#include "Surface.h"

void KE::GL::Surface::paint() {
	// Поверхность перерисовывается, если она видима.
	if (visible) {
		// Если нужно, сначала делаем вычисления.
		if (this->points.empty()) {
			calculate();
		}

		// Устанавливаем цвета обеих сторон поверхности.
		glMaterialfv(GL_FRONT, GL_DIFFUSE, getFrontColor().rgb);
		glMaterialfv(GL_BACK, GL_DIFFUSE, getBackColor().rgb);

		// Двусторонняя ли поверхность?
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, this->showBackSide);

		// Связаны ли треугольники?
		glBegin(stripped ? GL_TRIANGLE_STRIP : GL_TRIANGLES);

		for (const auto &pt : this->points) {
			glNormal3fv(pt.normal);
			glVertex3fv(pt.vertex);
		}

		glEnd();
	}
}

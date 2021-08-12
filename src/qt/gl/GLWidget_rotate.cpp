#include "GLWidget.h"

const double STEP = 0.006;

#define		M(x,y)		this->currentMatrix[4*x+y]

void GLWidget::rotate(const QPoint &start, const QPoint &end) {
	const int dx = end.x() - start.x();
	const int dy = end.y() - start.y();

  double tmp;

	this->makeCurrent();

  tmp = M(0, 1); M(0, 1) = M(1, 0); M(1, 0) = tmp;
  tmp = M(0, 2); M(0, 2) = M(2, 0); M(2, 0) = tmp;
  tmp = M(2, 1); M(2, 1) = M(1, 2); M(1, 2) = tmp;
	glMultMatrixd(this->currentMatrix.get());

  tmp = M(0, 1); M(0, 1) = M(1, 0); M(1, 0) = tmp;
  tmp = M(0, 2); M(0, 2) = M(2, 0); M(2, 0) = tmp;
  tmp = M(2, 1); M(2, 1) = M(1, 2); M(1, 2) = tmp;
	if (dx != 0) {
		for (int i = 0; i < 3; ++i) {
			tmp = M(i, 2) * sin(dx * STEP) + M(i, 0) * cos(dx * STEP);
			M(i, 2) = M(i, 2) * cos(dx * STEP) - M(i, 0) * sin(dx * STEP);
			M(i, 0) = tmp;
		}
	}
	if (dy != 0) {
		for (int i = 0; i < 3; ++i) {
			tmp = M(i, 1) * sin(dy * STEP) + M(i, 2) * cos(dy * STEP);
			M(i, 1) = M(i, 1) * cos(dy * STEP) - M(i, 2) * sin(dy * STEP);
			M(i, 2) = tmp;
		}
	}

	glMultMatrixd(this->currentMatrix.get());

	this->update();
}

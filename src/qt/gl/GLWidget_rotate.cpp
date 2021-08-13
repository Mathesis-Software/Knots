#include "GLWidget.h"

#include <QtWidgets/QApplication>

const double STEP = 0.006;

#define		M(x,y)		this->currentMatrix[4*x+y]

void GLWidget::rotate(const QPoint &start, const QPoint &end) {
	int dx = end.x() - start.x();
	int dy = end.y() - start.y();
	int dz = 0;
	const auto modifiers = QApplication::queryKeyboardModifiers();

	if (modifiers & Qt::AltModifier) {
		dz = dx;
		dx = 0;
		dy = 0;
	} else if (modifiers & Qt::ShiftModifier) {
		if (abs(dx) > abs(dy)) {
			dy = 0;
		} else {
			dx = 0;
		}
	}

	this->makeCurrent();

	double tmp;
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
	if (dz != 0) {
		for (int i = 0; i < 3; ++i) {
			tmp = M(i, 0) * sin(dz * STEP) + M(i, 1) * cos(dz * STEP);
			M(i, 0) = M(i, 0) * cos(dz * STEP) - M(i, 1) * sin(dz * STEP);
			M(i, 1) = tmp;
		}
	}

	glMultMatrixd(this->currentMatrix.get());

	this->update();
}

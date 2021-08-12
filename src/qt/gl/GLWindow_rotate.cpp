#include <cmath>

#include <QtCore/QTimerEvent>

#include "GLWindow.h"

const double STEP = 0.02;

#define		M(x,y)		this->currentMatrix[4*x+y]

void GLWindow::rotate(int num) {
  int axis1, axis2;

  switch (num) {
    default:
    case 0: axis1 = 1; axis2 = 0; break;
    case 1: axis1 = 0; axis2 = 1; break;
    case 2: axis1 = 2; axis2 = 0; break;
    case 3: axis1 = 0; axis2 = 2; break;
    case 4: axis1 = 2; axis2 = 1; break;
    case 5: axis1 = 1; axis2 = 2; break;
  }

	this->glWidget()->rotate(axis1, axis2);
}

void GLWidget::rotate(int axis1, int axis2) {
  double tmp;
  int i;

  tmp = M(0, 1); M(0, 1) = M(1, 0); M(1, 0) = tmp;
  tmp = M(0, 2); M(0, 2) = M(2, 0); M(2, 0) = tmp;
  tmp = M(2, 1); M(2, 1) = M(1, 2); M(1, 2) = tmp;
  this->multMatrix();
  tmp = M(0, 1); M(0, 1) = M(1, 0); M(1, 0) = tmp;
  tmp = M(0, 2); M(0, 2) = M(2, 0); M(2, 0) = tmp;
  tmp = M(2, 1); M(2, 1) = M(1, 2); M(1, 2) = tmp;

  for (i = 0; i < 3; i++) {
    tmp = M(i, axis1) * sin(STEP) + M(i, axis2) * cos(STEP);
    M(i, axis1) = M(i, axis1) * cos(STEP) - M(i, axis2) * sin(STEP);
    M(i, axis2) = tmp;
  }

  this->multMatrix();
	if (this->isVisible()) {
		this->update();
	}
}

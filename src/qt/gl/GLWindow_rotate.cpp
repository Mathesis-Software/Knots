#include <cmath>

#include <QtCore/QTimerEvent>

#include "GLWindow.h"

const double STEP = 0.02;

#define		M(x,y)		currentMatrix[4*x+y]
#define		S(x,y)		currentSpeedMatrix[3*x+y]

void GLWindow::inertia() {
  if (isInertia) {
    killTimer(timerId_rotate);
    isInertia = false;
    for (int i = 0; i < 3; i++)
      for (int j = 0; j < 3; j++)
        S(i, j) = (i == j) ? 1.0 : 0.0;
  } else {
    timerId_rotate = startTimer(1);
    isInertia = true;
  }
}

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

  if (isInertia)
    changeSpeed(axis1, axis2);
  else
    rotate(axis1, axis2);

  isSaved = false;
}

void GLWindow::changeSpeed(int axis1, int axis2) {
  double tmp;

  for (int i = 0; i < 3; i++) {
    tmp = S(i, axis1) * sin(STEP) + S(i, axis2) * cos(STEP);
    S(i, axis1) = S(i, axis1) * cos(STEP) - S(i, axis2) * sin(STEP);
    S(i, axis2) = tmp;
  }
}

void GLWindow::rotate(int axis1, int axis2) {
  double tmp;
  int i;

  tmp = M(0, 1); M(0, 1) = M(1, 0); M(1, 0) = tmp;
  tmp = M(0, 2); M(0, 2) = M(2, 0); M(2, 0) = tmp;
  tmp = M(2, 1); M(2, 1) = M(1, 2); M(1, 2) = tmp;
  ((GLWidget*)centralWidget())->multMatrix();
  tmp = M(0, 1); M(0, 1) = M(1, 0); M(1, 0) = tmp;
  tmp = M(0, 2); M(0, 2) = M(2, 0); M(2, 0) = tmp;
  tmp = M(2, 1); M(2, 1) = M(1, 2); M(1, 2) = tmp;

  for (i = 0; i < 3; i++) {
    tmp = M(i, axis1) * sin(STEP) + M(i, axis2) * cos(STEP);
    M(i, axis1) = M(i, axis1) * cos(STEP) - M(i, axis2) * sin(STEP);
    M(i, axis2) = tmp;
  }

  ((GLWidget*)centralWidget())->multMatrix();
  repaint3d();
}

void GLWindow::doRotate() {
  {
    double tmp;
    tmp = M(0, 1); M(0, 1) = M(1, 0); M(1, 0) = tmp;
    tmp = M(0, 2); M(0, 2) = M(2, 0); M(2, 0) = tmp;
    tmp = M(2, 1); M(2, 1) = M(1, 2); M(1, 2) = tmp;
    ((GLWidget*)centralWidget())->multMatrix();
    tmp = M(0, 1); M(0, 1) = M(1, 0); M(1, 0) = tmp;
    tmp = M(0, 2); M(0, 2) = M(2, 0); M(2, 0) = tmp;
    tmp = M(2, 1); M(2, 1) = M(1, 2); M(1, 2) = tmp;
  }

  {
    double tmp[3];
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++)
        tmp[j] = M(i, 0) * S(0, j) + M(i, 1) * S(1, j) + M(i, 2) * S(2, j);
      for (int j = 0; j < 3; j++)
        M(i, j) = tmp[j];
    }
  }

  ((GLWidget*)centralWidget())->multMatrix();
  repaint3d();
}

void GLWindow::timerEvent(QTimerEvent *te) {
  if (isInertia && (te->timerId() == timerId_rotate))
    doRotate();
}

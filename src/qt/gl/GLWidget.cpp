#include <QtGui/QMouseEvent>

#include "GLWidget.h"

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent), currentMatrix(new double[16]) {
	this->backgroundRGB[0] = 1.0;
	this->backgroundRGB[1] = 1.0;
	this->backgroundRGB[2] = 1.0;

	for (int i = 0; i < 16; ++i) {
		this->currentMatrix[i] = (i % 5) ? 0.0 : 1.0;
	}
}

void GLWidget::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);
  
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-w / 800.0, w / 800.0, -h / 800.0, h / 800.0, 5, 25);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -10.0);
  glMultMatrixd(this->currentMatrix.get());
}

void GLWidget::initializeGL() {
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

  float pos[] = {5.0, 5.0, 10.0, 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, pos);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);
}

void GLWidget::paintGL() {
  glClearColor(this->backgroundRGB[0], this->backgroundRGB[1], this->backgroundRGB[2], 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (auto surface : this->surfaces) {
    surface->paint();
	}

  glFlush();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
	this->capturedPoint = event->pos();
	this->selectMouseCursor();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
	if (!this->capturedPoint.isNull()) {
		this->rotate(this->capturedPoint, event->pos());
		this->capturedPoint = event->pos();
	}
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
	if (!this->capturedPoint.isNull()) {
		this->rotate(this->capturedPoint, event->pos());
	}
	this->capturedPoint = QPoint();
	this->selectMouseCursor();
}

void GLWidget::selectMouseCursor() {
	if (!this->capturedPoint.isNull()) {
		this->setCursor(Qt::DragMoveCursor);
	} else {
		this->unsetCursor();
	}
}

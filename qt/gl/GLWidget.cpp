#include "GLWindow.h"

GLWidget::GLWidget(GLWindow *p) : QOpenGLWidget(p) {
  Parent = p;
}

void GLWidget::resizeGL(int w, int h) {
  glViewport(0, 0, w, h);

  float pos[] = {5.0, 5.0, 10.0, 1.0};
  glLightfv(GL_LIGHT0, GL_POSITION, pos);
  glEnable(GL_LIGHT0);

  glEnable(GL_LIGHTING);
  
  glShadeModel(GL_SMOOTH);
  glEnable(GL_DEPTH_TEST);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-w/800.0, w/800.0, -h/800.0, h/800.0, 5, 25);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0, 0.0, -10.0);
  glMultMatrixd(Parent->currentMatrix.get());
}

void GLWidget::initializeGL() {
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

void GLWidget::paintGL() {
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(Parent->backgroundRGB[0], Parent->backgroundRGB[1], Parent->backgroundRGB[2], 1.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (auto surface : Parent->surfaces) {
    surface->paint();
	}

  glFlush();
}

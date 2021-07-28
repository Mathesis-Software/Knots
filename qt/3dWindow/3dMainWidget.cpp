#include "3dWindow.h"

dddMainWidget::dddMainWidget (dddWindow *p)
  : QOpenGLWidget (p)
{
  Parent = p;
  //setBackgroundColor (Qt::white);
}

dddMainWidget::~dddMainWidget (void)
{
}

void dddMainWidget::resizeGL (int w, int h)
{
  glViewport (0, 0, w, h);

  float pos [] = {5.0, 5.0, 10.0, 1.0};
  glLightfv (GL_LIGHT0, GL_POSITION, pos);
  glEnable (GL_LIGHT0);

  glEnable (GL_LIGHTING);
  
  glShadeModel (GL_SMOOTH);
  glEnable (GL_DEPTH_TEST);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  glFrustum (-w/800.0, w/800.0, -h/800.0, h/800.0, 5, 25);

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  glTranslatef (0.0, 0.0, -10.0);
  glMultMatrixd (Parent -> currentMatrix);
}

void dddMainWidget::paintGL (void)
{
  glClearColor (Parent -> backgroundRGB [0],
                Parent -> backgroundRGB [1],
		Parent -> backgroundRGB [2],
		1.0);
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  for (auto surface : Parent->surfaces) {
    surface->paint();
	}

  glFlush();
}

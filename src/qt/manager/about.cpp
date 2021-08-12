#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <QtCore/QString>
#include <fstream>
#include "about.h"

aboutWindow::aboutWindow (QWidget *parent) : QWidget(parent) {
  this->setWindowModality(Qt::ApplicationModal);
  setFixedSize (300, 120);
  setWindowTitle ("About Knot Editor");
  QPalette pal = this->palette();
  pal.setColor(QPalette::Background, Qt::white);
  this->setAutoFillBackground(true);
  this->setPalette(pal);
  timer_id = startTimer (100);

  picture = new pictureWidget (this);
  picture -> setGeometry (10, 0, 120, 120);
  picture -> update ();

  text = new textWidget (this);
  text -> setGeometry (130, 0, 150, 120);
  QFont fnt ("Helvetica", 14);
  text -> setFont (fnt);
  text -> setAlignment (Qt::AlignCenter);
  char aboutText [128];
  sprintf (aboutText, "Knot Editor\nversion %s", VERSION);
  text -> setText (aboutText);
}

aboutWindow::~aboutWindow (void)
{
  killTimer (this->timer_id);
  delete picture;
  delete text;
}

void aboutWindow::timerEvent(QTimerEvent*) {
  picture -> update ();
}

aboutSurface::aboutSurface(const char *filename) : Surface(true, false) {
  show();

	std::ifstream is(filename);

  float p[6];

  if (is) {
    while (!is.eof()) {
      is >> p[0] >> p[1] >> p[2] >> p[3] >> p[4] >> p[5];
      if (is.good())
        addpoint(KE::ThreeD::Point(p[0], p[1], p[2]), KE::ThreeD::Vector(p[3], p[4], p[5]));
      else
        break;
    }
    is.close();
  }
}

pictureWidget::pictureWidget (QWidget *parent) : QOpenGLWidget (parent) {
  //setBackgroundColor (white);

  aboutSurf = new aboutSurface((std::string(getenv("KNOTEDITOR_ROOT")) + "/icons/about.srf").c_str());

  srandom (time (NULL));
  x = 1.0;
  y = 0.0;
  z = 0.0;
}

pictureWidget::~pictureWidget (void)
{
  delete aboutSurf;
}

void pictureWidget::resizeGL (int w, int h)
{
  glViewport (0, 0, w, h);

  float pos[] = {5.0, 5.0, 10.0, 1.0};
  glLightfv (GL_LIGHT0, GL_POSITION, pos);
  glEnable (GL_LIGHT0);
  
  glEnable (GL_LIGHTING);

  glShadeModel (GL_SMOOTH);
  glEnable (GL_DEPTH_TEST);

  glMatrixMode (GL_PROJECTION);
  glLoadIdentity ();
  glFrustum (-0.7, 0.7, -0.7, 0.7, 5, 25);

  glMatrixMode (GL_MODELVIEW);
  glLoadIdentity ();
  glTranslatef (0.0, 0.0, -10.0);

  glClearColor (1.0, 1.0, 1.0, 1.0);
}

void pictureWidget::paintGL ()
{
  x += 0.6 * random () / RAND_MAX - 0.3;
  y += 0.6 * random () / RAND_MAX - 0.3;
  z += 0.6 * random () / RAND_MAX - 0.3;
  float len = sqrt (x * x + y * y + z * z);
  x /= len;
  y /= len;
  z /= len;
  
  makeCurrent ();
  glRotatef (5.0, x, y, z);
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  aboutSurf -> paint ();
  glFlush();
}

textWidget::textWidget (QWidget *parent)
 : QLabel (parent)
{
  //setBackgroundColor (white);
}

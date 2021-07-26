#ifndef __ABOUT_H__
#define __ABOUT_H__

#include <QtWidgets/qwidget.h>
#include <QtWidgets/qlabel.h>
#include <QtOpenGL/qgl.h>

#include "../../gl/surface/surface.h"

class aboutWindow : public QWidget
{

Q_OBJECT

public:
  
  aboutWindow (QWidget*);
  ~aboutWindow (void);

private:

  class pictureWidget *picture;
  class textWidget *text;

  void timerEvent (QTimerEvent*);
  void mousePressEvent (QMouseEvent*)
    {delete this;};

	int timer_id;
};

class aboutSurface : public surface
{

public:

  aboutSurface (const char*);
  void calculate (void) {};
};

class pictureWidget : public QGLWidget
{

Q_OBJECT

public:

  pictureWidget (QWidget*);
  ~pictureWidget (void);

private:

  surface *aboutSurf;
  float x, y, z;
  
  void resizeGL (int, int);
  void paintGL ();
  void mousePressEvent (QMouseEvent*)
    {delete parent ();};
};

class textWidget : public QLabel
{

Q_OBJECT

public:

  textWidget (QWidget*);
  ~textWidget (void) {};

private:

  void mousePressEvent (QMouseEvent*)
    {delete parent ();};
};

#endif /* __ABOUT_H__ */

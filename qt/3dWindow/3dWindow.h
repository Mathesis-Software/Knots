#ifndef __3DWINDOW_H__
#define __3DWINDOW_H__

#include <list>

#include <QtWidgets/QOpenGLWidget>

#include "../setupColor/setupColor.h"
#include "../abstractWindow/abstractWindow.h"
#include "../../gl/surface/surface.h"

class dddWindow : public abstractWindow {

  Q_OBJECT

private:

	std::list<surface*> *surfaces;

  double *currentSpeedMatrix;
  double *currentMatrix;

  bool isInertia;
  int timerId_rotate;

  void printIt (QPrinter*);

  void rotate (int, int);
  void changeSpeed (int, int);
  void doRotate (void);

private slots:

  void inertia ();
  void rotate (int);

protected:

  float backgroundRGB[3];

  void timerEvent (QTimerEvent*);

  void addSurface (surface *s)
    {surfaces -> push_back (s);};

  const double currMatr (int i, int j)
    {return currentMatrix [4 * i + j];}

  void repaint3d (void);

public:
  
  dddWindow (void);
  ~dddWindow (void);

  friend class dddMainWidget;
  friend class setup3dMainWidgetColor;
};

class dddMainWidget : public QOpenGLWidget {

private:

  dddWindow *Parent;
  void resizeGL (int, int);
  void paintGL (void);

public:

  dddMainWidget (dddWindow*);
  ~dddMainWidget (void);

  void multMatrix (void)
    {makeCurrent ();
     glMultMatrixd (Parent -> currentMatrix);}
};

class setup3dMainWidgetColor : public setupColor {

  Q_OBJECT

private:

  dddWindow *Parent;

private slots:
  
  void apply ()
    {Parent -> repaint3d ();};
    
public:
  
  setup3dMainWidgetColor (dddWindow *p, float *rgb, bool just = true)
    : setupColor (rgb, just) {Parent = p;};
};

#endif /* __3DWINDOW_H__ */

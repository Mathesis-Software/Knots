#include <QtGui/qpixmap.h>
#include <QtGui/qpainter.h>
//#include <qprinter.h>
#include <QtWidgets/qtoolbar.h>
#include <QtWidgets/qtoolbutton.h>
#include <QtWidgets/qbuttongroup.h>

#include "3dWindow.h"

dddWindow::dddWindow (void)
{
  setCentralWidget (new dddMainWidget (this));

  backgroundRGB [0] = 1.0;
  backgroundRGB [1] = 1.0;
  backgroundRGB [2] = 1.0;
  surfaces = new std::list<surface*>;

  currentMatrix = new double [16];
  currentSpeedMatrix = new double [9];
  {
    int i;
    for (i = 0; i < 16; i++)
      currentMatrix [i] = (i % 5) ? 0.0 : 1.0;
    for (i = 0; i < 9; i++)
      currentSpeedMatrix [i] = (i % 4) ? 0.0 : 1.0;
  }
  isInertia = false;

  addToolBarSeparator ();

  QToolButton **rotateButton = new QToolButton* [6];
  rotateButton [0] = addToolBarButton ("rotate0.xpm", "Rotate", 0);
  rotateButton [1] = addToolBarButton ("rotate1.xpm", "Rotate", 0);
  rotateButton [2] = addToolBarButton ("rotate2.xpm", "Rotate", 0);
  rotateButton [3] = addToolBarButton ("rotate3.xpm", "Rotate", 0);
  rotateButton [4] = addToolBarButton ("rotate4.xpm", "Rotate", 0);
  rotateButton [5] = addToolBarButton ("rotate5.xpm", "Rotate", 0);
  QButtonGroup *grp = new QButtonGroup;
  for (int i = 0; i < 6; i++)
    grp -> addButton (rotateButton [i]);
  connect (grp, SIGNAL (pressed(int)), SLOT (rotate(int)) );
  delete[] rotateButton;

  addToolBarSeparator ();
  addToolBarButton ("inertia.xpm", "Inertia", SLOT (inertia()))
    -> setCheckable (true);
}

dddWindow::~dddWindow (void)
{
  delete[] currentMatrix;
  delete[] currentSpeedMatrix;

  for (std::list<surface*>::iterator it = surfaces -> begin ();
       it != surfaces -> end ();
       it++)
    delete (*it);
  delete surfaces;
}

/*
void dddWindow::printIt (QPrinter *prn)
{
  QPainter pnt;
  pnt.begin (prn);
  raise ();
  pnt.drawPixmap (0, 0, QPixmap::grabWindow (centralWidget () -> winId ()));
  pnt.end ();
}*/

void dddWindow::repaint3d (void)
{
  if (isVisible ())
    ((dddMainWidget*) centralWidget ()) -> update ();
}

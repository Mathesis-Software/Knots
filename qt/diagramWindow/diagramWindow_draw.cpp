#include <math.h>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>

#include "diagramWindow.h"

diagramMainWidget::diagramMainWidget(diagramWindow *p) : QWidget (p) {
  Parent = p;
}

void diagramMainWidget::drawPoint (QPainter *p, vertex *v)
{
  p -> drawEllipse (v -> x () - 4, v -> y () - 4, 9, 9);
}

void diagramMainWidget::drawEdge (QPainter *p, vertex *v)
{
  float DeltaX = v -> next () -> x () - v -> x ();
  float DeltaY = v -> next () -> y () - v -> y ();
  float hyp = hypot (DeltaX, DeltaY);

  DeltaX = 10 * DeltaX / hyp;
  DeltaY = 10 * DeltaY / hyp;

  int x0 = v -> x (),
      y0 = v -> y (),
      x1, y1;
  crossing *crs = v -> crs ();

  while (crs)
  {
    if ( (Parent -> isClosed) || (crs -> up () != Parent -> base -> prev ()) )
    {
      x1 = (int) (crs -> x () - DeltaX);
      y1 = (int) (crs -> y () - DeltaY);

      if ( (x1 - x0) * DeltaX + (y1 - y0) * DeltaY > 0 )
        p -> drawLine (x0, y0, x1, y1);

      x0 = (int) (crs -> x () + DeltaX);
      y0 = (int) (crs -> y () + DeltaY);
    }

    crs = crs -> next ();
  }

  x1 = v -> next () -> x ();
  y1 = v -> next () -> y ();

  if ( (x1 - x0) * DeltaX + (y1 - y0) * DeltaY > 0 )
    p -> drawLine (x0, y0, x1, y1);
}

void diagramMainWidget::drawIt (QPainter *p)
{
  if (Parent -> isEmpty ())
    return;

  p -> setPen (Qt::black);
  p -> setBrush (Qt::black);
  
  vertex *v = Parent -> base;

  do
  {
    drawPoint (p, v);
    v = v -> next ();
  }
  while (v != Parent -> base);

  do
  {
    drawEdge (p, v);
    v = v -> next ();
  }
  while (Parent -> isClosed ? (v != Parent -> base) : (v -> next () != Parent -> base));
}

void diagramMainWidget::paintEvent(QPaintEvent*) {
  QPixmap pm(width(), height());
  pm.fill(Qt::white);
  QPainter pnt;
  pnt.begin(&pm);
  drawIt(&pnt);
  pnt.end();
  QPainter main;
  main.begin(this);
  main.drawPixmap(0, 0, pm);
  main.end();
}

void diagramWindow::printIt (QPrinter *prn) {
  QPainter pnt;
  pnt.begin (prn);
  ( (diagramMainWidget*) centralWidget () ) -> drawIt (&pnt);
  pnt.end ();
}

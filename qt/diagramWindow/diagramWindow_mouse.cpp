#include <QtGui/QMouseEvent>

#include "diagramWindow.h"

static vertex *localVertex = NULL;
static int localx, localy;
static bool doSomething = false;

void diagramMainWidget::mousePressEvent(QMouseEvent *m) {
  switch (Parent -> mode) {
    case diagramWindow::DRAW_NEW_DIAGRAM:
    {
      if (Parent -> isEmpty ())
        Parent -> actions_clear -> setEnabled (true);
      if (Parent -> isClosed)
        return;
      Parent -> addVertex (NULL, m -> x (), m -> y ());
      if (m -> button () == 0x02)
      {
        Parent -> isClosed = true;
        Parent -> actions_convert -> setEnabled (true);
        Parent -> actions_simplify -> setEnabled (true);
	Parent -> actions[0] -> setChecked (false);
      }
      repaint ();
      //repaint (false);
      Parent -> isSaved = false;
      doSomething = true;
      localVertex = Parent -> base -> prev ();
      return;
    }

    case diagramWindow::ADD_POINT:
    {
      vertex *v = Parent -> nearToEdge (m -> x (), m -> y ());
      if (v)
      {
        Parent -> addVertex (v, m -> x (), m -> y ());
        localVertex = v -> next ();
        repaint ();
        //repaint (false);
        Parent -> isSaved = false;
        doSomething = true;
      }
      return;
    }

    case diagramWindow::MOVE_POINT:
    {
      vertex *v = Parent -> nearToVertex (m -> x (), m -> y ());
      if (v)
      {
        localVertex = v;
        Parent -> isSaved = false;
        doSomething = true;
      }
      return;
    }

    case diagramWindow::REMOVE_POINT:
    {
      vertex *v = Parent -> nearToVertex (m -> x (), m -> y ());
      if (v)
      {
        Parent -> removeVertex (v);
	if (Parent -> isEmpty ())
	  Parent -> clear ();
	else
	{
          repaint ();
          //repaint (false);
          Parent -> isSaved = false;
	}
      }
      return;
    }
    
    case diagramWindow::CHANGE_CROSS:
    {
      crossing *c = Parent -> nearToCross (m -> x (), m -> y ());
      if (c)
      {
        Parent -> tryChangeCrossing (c -> up (), c -> down ());
        repaint ();
        //repaint (false);
        Parent -> isSaved = false;
      }
      return;
    }

    case diagramWindow::MOVE_DIAGRAM:
    {
      localx = m -> x ();
      localy = m -> y ();
      Parent -> isSaved = false;
      doSomething = true;
      return;
    }

    default:
      return;
  }
}

void diagramMainWidget::mouseReleaseEvent (QMouseEvent *m)
{
  if (!doSomething)
    return;

  doSomething = false;

  switch (Parent -> mode)
  {
    case diagramWindow::ADD_POINT:
    case diagramWindow::MOVE_POINT:
    case diagramWindow::DRAW_NEW_DIAGRAM:
    {
      Parent -> moveVertex (localVertex, m -> x(), m -> y());
      repaint ();
      //repaint (false);
      return;
    }

    case diagramWindow::MOVE_DIAGRAM:
    {
      Parent -> shift (m -> x() - localx, m -> y () - localy);
      localx = m -> x ();
      localy = m -> y ();
      repaint ();
      //repaint (false);
      return;
    }

    default:
      return;
  }
}

void diagramMainWidget::mouseMoveEvent (QMouseEvent *m)
{
  if (!doSomething)
    return;

  switch (Parent -> mode)
  {
    case diagramWindow::ADD_POINT:
    case diagramWindow::MOVE_POINT:
    case diagramWindow::DRAW_NEW_DIAGRAM:
    {
      Parent -> moveVertex (localVertex, m -> x(), m -> y());
      repaint ();
      //repaint (false);
      return;
    }

    case diagramWindow::MOVE_DIAGRAM:
    {
      Parent -> shift (m -> x() - localx, m -> y () - localy);
      repaint ();
      //repaint (false);
      localx = m -> x ();
      localy = m -> y ();
      return;
    }

    default:
      return;
  }
}

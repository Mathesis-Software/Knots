#include <QtGui/QMouseEvent>

#include "diagramWindow.h"

static KE::TwoD::Diagram::Vertex *localVertex = NULL;
static int localx, localy;
static bool doSomething = false;

void diagramMainWidget::mousePressEvent(QMouseEvent *m) {
  switch (Parent->mode) {
    case diagramWindow::DRAW_NEW_DIAGRAM:
      if (Parent->isEmpty()) {
        Parent->actions_clear->setEnabled (true);
			}
      if (Parent->diagram.isClosed) {
        return;
			}
      Parent->diagram.addVertex(NULL, m->x(), m->y());
      if (m->button() == 0x02) {
        Parent->diagram.isClosed = true;
        Parent->actions_convert->setEnabled(true);
        Parent->actions_simplify->setEnabled(true);
				Parent->actions[0]->setChecked(false);
      }
      repaint();
      Parent->isSaved = false;
      doSomething = true;
      localVertex = Parent->diagram.vertices().back();
      return;
    case diagramWindow::ADD_POINT:
    {
			std::shared_ptr<KE::TwoD::Diagram::Edge> edge = Parent->diagram.findEdge(m->x(), m->y(), 5);
      if (edge) {
        localVertex = Parent->diagram.addVertex(edge->start, m->x(), m->y());
        repaint();
        Parent->isSaved = false;
        doSomething = true;
      }
      return;
    }
    case diagramWindow::MOVE_POINT:
    {
			KE::TwoD::Diagram::Vertex *v = Parent->diagram.findVertex(m->x(), m->y(), 17);
      if (v) {
        localVertex = v;
        Parent->isSaved = false;
        doSomething = true;
      }
      return;
    }
    case diagramWindow::REMOVE_POINT:
    {
			KE::TwoD::Diagram::Vertex *v = Parent->diagram.findVertex(m->x(), m->y(), 17);
      if (v) {
        Parent->diagram.removeVertex(v);
				if (Parent->isEmpty()) {
					Parent->clear();
				} else {
          repaint();
          //repaint(false);
          Parent->isSaved = false;
				}
      }
      return;
    }
    case diagramWindow::CHANGE_CROSS:
    {
      auto c = Parent->diagram.findCrossing(m->x(), m->y(), 17);
      if (c) {
        Parent->diagram.tryChangeCrossing(c->up(), c->down());
        repaint();
        //repaint(false);
        Parent->isSaved = false;
      }
      return;
    }
    case diagramWindow::MOVE_DIAGRAM:
      localx = m->x();
      localy = m->y();
      Parent->isSaved = false;
      doSomething = true;
      return;
    default:
      return;
  }
}

void diagramMainWidget::mouseReleaseEvent(QMouseEvent *m) {
  if (!doSomething)
    return;

  doSomething = false;

  switch (Parent->mode) {
    case diagramWindow::ADD_POINT:
    case diagramWindow::MOVE_POINT:
    case diagramWindow::DRAW_NEW_DIAGRAM:
      Parent->diagram.moveVertex(localVertex, m->x(), m->y());
      repaint();
      //repaint(false);
      return;
    case diagramWindow::MOVE_DIAGRAM:
      Parent->diagram.shift(m->x() - localx, m->y() - localy);
      localx = m->x();
      localy = m->y();
      repaint();
      //repaint(false);
      return;
    default:
      return;
  }
}

void diagramMainWidget::mouseMoveEvent(QMouseEvent *m) {
  if (!doSomething)
    return;

  switch (Parent->mode) {
    case diagramWindow::ADD_POINT:
    case diagramWindow::MOVE_POINT:
    case diagramWindow::DRAW_NEW_DIAGRAM:
      Parent->diagram.moveVertex(localVertex, m->x(), m->y());
      repaint();
      //repaint(false);
      return;
    case diagramWindow::MOVE_DIAGRAM:
      Parent->diagram.shift(m->x() - localx, m->y() - localy);
      repaint();
      //repaint(false);
      localx = m->x();
      localy = m->y();
      return;
    default:
      return;
  }
}

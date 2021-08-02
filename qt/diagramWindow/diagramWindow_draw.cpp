#include <math.h>
#include <QtGui/QPainter>
#include <QtGui/QPixmap>

#include "diagramWindow.h"

diagramMainWidget::diagramMainWidget(diagramWindow *p) : QWidget (p) {
  Parent = p;
}

void diagramMainWidget::drawVertex(QPainter &painter, const KE::TwoD::Diagram::Vertex &v) {
  painter.drawEllipse(v.x() - 4, v.y() - 4, 9, 9);
}

void diagramMainWidget::drawEdge(QPainter &painter, const KE::TwoD::Diagram::Edge &edge) {
  float deltaX = edge.end->x() - edge.start->x();
  float deltaY = edge.end->y() - edge.start->y();
  float hyp = hypotf(deltaX, deltaY);

  deltaX = 10 * deltaX / hyp;
  deltaY = 10 * deltaY / hyp;

  float x0 = edge.start->x(),
				y0 = edge.start->y(),
				x1, y1;

	for (const auto &crs : edge.start->crossings()) {
		// TODO: remove this check; there should be no invisible edges
    if (Parent->diagram.isClosed || crs.up.start != Parent->diagram.vertices().back()) {
			auto coords = crs.coords();
			if (!coords) {
				continue;
			}
      x1 = coords->x - deltaX;
      y1 = coords->y - deltaY;

      if ((x1 - x0) * deltaX + (y1 - y0) * deltaY > 0) {
        painter.drawLine(x0, y0, x1, y1);
			}

      x0 = coords->x + deltaX;
      y0 = coords->y + deltaY;
    }
  }

  x1 = edge.end->x();
  y1 = edge.end->y();

  if ((x1 - x0) * deltaX + (y1 - y0) * deltaY > 0) {
    painter.drawLine(x0, y0, x1, y1);
	}
}

void diagramMainWidget::drawIt(QPainter &painter) {
  if (Parent->isEmpty())
    return;

  painter.setPen(Qt::black);
  painter.setBrush(Qt::black);
  
	for (auto vertex : Parent->diagram.vertices()) {
    drawVertex(painter, *vertex);
  }
	for (const auto &edge : Parent->diagram.edges()) {
    drawEdge(painter, edge);
	}
}

void diagramMainWidget::paintEvent(QPaintEvent*) {
  QPixmap pm(width(), height());
  pm.fill(Qt::white);
  QPainter pnt;
  pnt.begin(&pm);
  drawIt(pnt);
  pnt.end();
  QPainter main;
  main.begin(this);
  main.drawPixmap(0, 0, pm);
  main.end();
}

void diagramWindow::printIt(QPrinter *prn) {
  QPainter pnt;
  pnt.begin(prn);
  ((diagramMainWidget*)centralWidget())->drawIt(pnt);
  pnt.end();
}

#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>

#include "diagramWindow.h"

static int localx, localy;
static bool doSomething = false;

DiagramWidget::DiagramWidget(diagramWindow *p) : QWidget(p), _editingMode(NEW_DIAGRAM) {
	Parent = p;
	this->setMouseTracking(true);
}

DiagramWidget::DiagramWidget(diagramWindow *p, const rapidjson::Document &doc) : QWidget(p), diagram(doc), _editingMode(NEW_DIAGRAM) {
	Parent = p;
	this->setMouseTracking(true);
}

bool DiagramWidget::canSetEditingMode(DiagramWidget::EditingMode mode) const {
	switch (mode) {
		default:
		case NEW_DIAGRAM:
			return !this->diagram.isClosed();
		case ADD_VERTEX:
			return this->diagram.vertices().size() > 1;
		case MOVE_VERTEX:
		case REMOVE_VERTEX:
		case MOVE_DIAGRAM:
			return !this->diagram.vertices().empty();
		case FLIP_CROSSING:
			return this->diagram.hasCrossings();
	}
}

bool DiagramWidget::setEditingMode(DiagramWidget::EditingMode mode) {
	if (mode != this->_editingMode && this->canSetEditingMode(mode)) {
		this->_editingMode = mode;
		this->setCapturedVertex(nullptr);
		this->setCapturedEdge(nullptr);
		this->setCapturedCrossing(nullptr);
		return true;
	} else {
		return false;
	}
}

void DiagramWidget::clear() {
	this->diagram.clear();
	this->repaint();
	this->setEditingMode(NEW_DIAGRAM);
}

void DiagramWidget::highlightCrossing(QPainter &painter, const KE::TwoD::Diagram::Crossing &crossing) {
	painter.setPen(Qt::lightGray);
	painter.setBrush(Qt::lightGray);

	const auto coords = crossing.coords();
	if (coords) {
		painter.drawEllipse(QPointF(coords->x, coords->y), 10, 10);
	}
}

void DiagramWidget::drawVertex(QPainter &painter, const std::shared_ptr<KE::TwoD::Diagram::Vertex> &vertex) {
	if (vertex == this->capturedVertex) {
		painter.setPen(Qt::lightGray);
		painter.setBrush(Qt::lightGray);
	} else {
		painter.setPen(Qt::black);
		painter.setBrush(Qt::black);
	}

	auto coords = vertex->coords();
	painter.drawEllipse(QPointF(coords.x, coords.y), 4.5, 4.5);
}

void DiagramWidget::drawEdge(QPainter &painter, const KE::TwoD::Diagram::Edge &edge) {
	if (this->capturedEdge && edge == *this->capturedEdge) {
		painter.setPen(Qt::lightGray);
	} else {
		painter.setPen(Qt::black);
	}

	float deltaX = edge.end->x() - edge.start->x();
	float deltaY = edge.end->y() - edge.start->y();
	float hyp = hypotf(deltaX, deltaY);

	deltaX = 10 * deltaX / hyp;
	deltaY = 10 * deltaY / hyp;

	float x0 = edge.start->x(),
				y0 = edge.start->y(),
				x1, y1;

	for (const auto &crs : this->diagram.crossings(edge)) {
		auto coords = crs.coords();
		if (!coords) {
			continue;
		}
		x1 = coords->x - deltaX;
		y1 = coords->y - deltaY;

		if ((x1 - x0) * deltaX + (y1 - y0) * deltaY > 0) {
			painter.drawLine(QPointF(x0, y0), QPointF(x1, y1));
		}

		x0 = coords->x + deltaX;
		y0 = coords->y + deltaY;
	}

	x1 = edge.end->x();
	y1 = edge.end->y();

	if ((x1 - x0) * deltaX + (y1 - y0) * deltaY > 0) {
		painter.drawLine(QPointF(x0, y0), QPointF(x1, y1));
	}
}

void DiagramWidget::drawIt(QPainter &painter) {
	if (this->capturedCrossing) {
		this->highlightCrossing(painter, *this->capturedCrossing);
	}
	for (const auto &edge : this->diagram.edges()) {
		drawEdge(painter, edge);
	}
	for (const auto &vertex : this->diagram.vertices()) {
		drawVertex(painter, vertex);
	}
}

void DiagramWidget::paintEvent(QPaintEvent*) {
	QPainter pnt;
	pnt.begin(this);
	pnt.setRenderHint(QPainter::Antialiasing);
	pnt.fillRect(0, 0, this->width(), this->height(), Qt::white);
	drawIt(pnt);
	pnt.end();
}

void DiagramWidget::mousePressEvent(QMouseEvent *m) {
	switch (this->_editingMode) {
		case NEW_DIAGRAM:
			if (this->diagram.isClosed()) {
				break;
			}
			this->setCapturedVertex(this->diagram.addVertex(m->x(), m->y()));
			if (m->button() == 0x02) {
				this->diagram.close();
			}
			Parent->isSaved = false;
			break;
		case ADD_VERTEX:
		{
			if (this->capturedEdge) {
				this->setCapturedVertex(this->diagram.addVertex(*this->capturedEdge, m->x(), m->y()));
				this->setCapturedEdge(nullptr);
				Parent->isSaved = false;
			}
			break;
		}
		case MOVE_VERTEX:
			if (this->capturedVertex) {
				Parent->isSaved = false;
			}
			break;
		case REMOVE_VERTEX:
			if (this->capturedVertex) {
				this->diagram.removeVertex(this->capturedVertex);
				this->setCapturedVertex(nullptr);
				if (this->diagram.vertices().empty()) {
					this->setEditingMode(NEW_DIAGRAM);
				}
				this->repaint();
				this->Parent->isSaved = false;
			}
			break;
		case FLIP_CROSSING:
		{
			if (this->capturedCrossing) {
				this->setCapturedCrossing(this->diagram.flipCrossing(*this->capturedCrossing));
				Parent->isSaved = false;
			}
			break;
		}
		case MOVE_DIAGRAM:
			localx = m->x();
			localy = m->y();
			Parent->isSaved = false;
			doSomething = true;
			break;
		default:
			break;
	}
	this->Parent->updateActions();
}

void DiagramWidget::mouseReleaseEvent(QMouseEvent *m) {
	switch (this->_editingMode) {
		case ADD_VERTEX:
		case MOVE_VERTEX:
		case NEW_DIAGRAM:
			if (this->capturedVertex) {
				this->diagram.moveVertex(this->capturedVertex, m->x(), m->y());
				this->setCapturedVertex(nullptr);
				repaint();
			}
			break;
		case MOVE_DIAGRAM:
			if (doSomething) {
				this->diagram.shift(m->x() - localx, m->y() - localy);
				localx = m->x();
				localy = m->y();
				repaint();
				doSomething = false;
			}
			break;
		default:
			break;
	}

	this->Parent->updateActions();
}

void DiagramWidget::mouseMoveEvent(QMouseEvent *m) {
	if (m->buttons() == 0) {
		switch (this->_editingMode) {
			case MOVE_VERTEX:
			case REMOVE_VERTEX:
				this->setCapturedVertex(this->diagram.findVertex(KE::TwoD::FloatPoint(m->x(), m->y()), 17));
				break;
			case ADD_VERTEX:
				this->setCapturedEdge(this->diagram.findEdge(KE::TwoD::FloatPoint(m->x(), m->y()), 5));
				break;
			case FLIP_CROSSING:
				this->setCapturedCrossing(this->diagram.findCrossing(KE::TwoD::FloatPoint(m->x(), m->y()), 17));
				break;
			default:
				break;
		}
		return;
	}

	switch (this->_editingMode) {
		case ADD_VERTEX:
		case MOVE_VERTEX:
		case NEW_DIAGRAM:
			if (this->capturedVertex) {
				this->diagram.moveVertex(this->capturedVertex, m->x(), m->y());
				repaint();
			}
			return;
		case MOVE_DIAGRAM:
			if (doSomething) {
				this->diagram.shift(m->x() - localx, m->y() - localy);
				repaint();
				localx = m->x();
				localy = m->y();
			}
			return;
		default:
			return;
	}
}

void DiagramWidget::setCapturedVertex(const std::shared_ptr<KE::TwoD::Diagram::Vertex> &vertex) {
	auto old = this->capturedVertex;
	this->capturedVertex = vertex;
	if (old != this->capturedVertex) {
		this->repaint();
	}
}

void DiagramWidget::setCapturedEdge(const std::shared_ptr<KE::TwoD::Diagram::Edge> &edge) {
	auto old = this->capturedEdge;
	this->capturedEdge = edge;
	if (old != this->capturedEdge) {
		this->repaint();
	}
}

void DiagramWidget::setCapturedCrossing(const std::shared_ptr<KE::TwoD::Diagram::Crossing> &crossings) {
	auto old = this->capturedCrossing;
	this->capturedCrossing = crossings;
	if (old != this->capturedCrossing) {
		this->repaint();
	}
}

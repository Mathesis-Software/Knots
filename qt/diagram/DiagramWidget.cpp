#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>

#include "diagramWindow.h"

static std::shared_ptr<KE::TwoD::Diagram::Vertex> localVertex;
static int localx, localy;
static bool doSomething = false;

DiagramWidget::DiagramWidget(diagramWindow *p) : QWidget(p), _editingMode(NEW_DIAGRAM) {
	Parent = p;
}

DiagramWidget::DiagramWidget(diagramWindow *p, const rapidjson::Document &doc) : QWidget(p), diagram(doc), _editingMode(NEW_DIAGRAM) {
	Parent = p;
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

void DiagramWidget::drawVertex(QPainter &painter, const KE::TwoD::Diagram::Vertex &v) {
	painter.drawEllipse(v.x() - 4, v.y() - 4, 9, 9);
}

void DiagramWidget::drawEdge(QPainter &painter, const KE::TwoD::Diagram::Edge &edge) {
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
	painter.setPen(Qt::black);
	painter.setBrush(Qt::black);
	
	for (auto vertex : this->diagram.vertices()) {
		drawVertex(painter, *vertex);
	}
	for (const auto &edge : this->diagram.edges()) {
		drawEdge(painter, edge);
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
			localVertex = this->diagram.addVertex(m->x(), m->y());
			if (m->button() == 0x02) {
				this->diagram.close();
			}
			repaint();
			Parent->isSaved = false;
			doSomething = true;
			break;
		case ADD_VERTEX:
		{
			std::shared_ptr<KE::TwoD::Diagram::Edge> edge = this->diagram.findEdge(KE::TwoD::FloatPoint(m->x(), m->y()), 5);
			if (edge) {
				localVertex = this->diagram.addVertex(*edge, m->x(), m->y());
				repaint();
				Parent->isSaved = false;
				doSomething = true;
			}
			break;
		}
		case MOVE_VERTEX:
		{
			const auto vertex = this->diagram.findVertex(KE::TwoD::FloatPoint(m->x(), m->y()), 17);
			if (vertex) {
				localVertex = vertex;
				Parent->isSaved = false;
				doSomething = true;
			}
			break;
		}
		case REMOVE_VERTEX:
		{
			const auto vertex = this->diagram.findVertex(KE::TwoD::FloatPoint(m->x(), m->y()), 17);
			if (vertex) {
				this->diagram.removeVertex(vertex);
				if (this->diagram.vertices().empty()) {
					this->setEditingMode(NEW_DIAGRAM);
				}
				this->repaint();
				this->Parent->isSaved = false;
			}
			break;
		}
		case FLIP_CROSSING:
		{
			auto c = this->diagram.findCrossing(KE::TwoD::FloatPoint(m->x(), m->y()), 17);
			if (c) {
				this->diagram.flipCrossing(*c);
				repaint();
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
	if (!doSomething)
		return;

	doSomething = false;

	switch (this->_editingMode) {
		case ADD_VERTEX:
		case MOVE_VERTEX:
		case NEW_DIAGRAM:
			this->diagram.moveVertex(localVertex, m->x(), m->y());
			repaint();
			break;
		case MOVE_DIAGRAM:
			this->diagram.shift(m->x() - localx, m->y() - localy);
			localx = m->x();
			localy = m->y();
			repaint();
			break;
		default:
			break;
	}

	this->Parent->updateActions();
}

void DiagramWidget::mouseMoveEvent(QMouseEvent *m) {
	if (!doSomething)
		return;

	switch (this->_editingMode) {
		case ADD_VERTEX:
		case MOVE_VERTEX:
		case NEW_DIAGRAM:
			this->diagram.moveVertex(localVertex, m->x(), m->y());
			repaint();
			return;
		case MOVE_DIAGRAM:
			this->diagram.shift(m->x() - localx, m->y() - localy);
			repaint();
			localx = m->x();
			localy = m->y();
			return;
		default:
			return;
	}
}

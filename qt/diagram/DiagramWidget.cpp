#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QStatusBar>

#include "diagramWindow.h"

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
		this->setFakeVertex(nullptr);
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

void DiagramWidget::drawVertex(QPainter &painter, const KE::TwoD::Diagram::Vertex &vertex, bool highlight) {
	if (highlight) {
		painter.setPen(Qt::lightGray);
		painter.setBrush(Qt::lightGray);
	} else {
		painter.setPen(Qt::black);
		painter.setBrush(Qt::black);
	}

	auto coords = vertex.coords();
	painter.drawEllipse(QPointF(coords.x, coords.y), 4.5, 4.5);
}

void DiagramWidget::drawEdge(QPainter &painter, const KE::TwoD::Diagram::Edge &edge, EdgeMode mode) {
	switch (mode) {
		case normal:
			painter.setPen(Qt::black);
			break;
		case highlighted:
		case fake:
			painter.setPen(Qt::lightGray);
			break;
		case fake2:
		{
			QPen pen(0xe0e0e0);
			pen.setDashPattern({6.0, 10.0});
			painter.setPen(pen);
			break;
		}
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
	if (this->capturedPoint.isNull()) {
		if (this->capturedCrossing) {
			this->highlightCrossing(painter, *this->capturedCrossing);
		}
		const auto vertices = this->diagram.vertices();
		auto fakeVertex = this->fakeVertex;
		if (fakeVertex && !vertices.empty()) {
			const KE::TwoD::Diagram::Edge edge(fakeVertex, vertices.back());
			drawEdge(painter, edge, EdgeMode::fake);
			if (vertices.size() > 1) {
				const KE::TwoD::Diagram::Edge edge2(fakeVertex, vertices.front());
				drawEdge(painter, edge2, EdgeMode::fake2);
			}
		}
		for (const auto &edge : this->diagram.edges()) {
			drawEdge(painter, edge, this->capturedEdge && edge == *this->capturedEdge ? EdgeMode::highlighted : EdgeMode::normal);
		}
		for (const auto &vertex : vertices) {
			drawVertex(painter, *vertex, vertex == this->capturedVertex);
		}
		if (fakeVertex) {
			drawVertex(painter, *fakeVertex, true);
		}
	} else {
		for (const auto &edge : this->diagram.edges()) {
			drawEdge(painter, edge, EdgeMode::highlighted);
		}
		for (const auto &vertex : this->diagram.vertices()) {
			drawVertex(painter, *vertex, true);
		}
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

void DiagramWidget::leaveEvent(QEvent *event) {
	if (this->fakeVertex) {
		this->setFakeVertex(nullptr);
		this->repaint();
	}
}

void DiagramWidget::mousePressEvent(QMouseEvent *event) {
	switch (this->_editingMode) {
		case NEW_DIAGRAM:
		{
			if (this->diagram.isClosed()) {
				this->setFakeVertex(nullptr);
				break;
			}
			auto fakeVertex = this->fakeVertex;
			if (fakeVertex) {
				this->setFakeVertex(nullptr);
				this->setCapturedVertex(this->diagram.addVertex(fakeVertex->x(), fakeVertex->y()));
				if (event->button() == 0x02) {
					this->diagram.close();
				}
				Parent->isSaved = false;
			}
			break;
		}
		case ADD_VERTEX:
			if (this->capturedEdge) {
				this->setCapturedVertex(this->diagram.addVertex(*this->capturedEdge, event->x(), event->y()));
				this->setCapturedEdge(nullptr);
				Parent->isSaved = false;
			}
			break;
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
			this->capturedPoint = event->pos();
			Parent->isSaved = false;
			break;
		default:
			break;
	}
	this->Parent->updateActions();
}

void DiagramWidget::mouseReleaseEvent(QMouseEvent *event) {
	switch (this->_editingMode) {
		case ADD_VERTEX:
		case MOVE_VERTEX:
		case NEW_DIAGRAM:
			if (this->capturedVertex) {
				this->diagram.moveVertex(this->capturedVertex, event->x(), event->y());
				this->setCapturedVertex(nullptr);
				repaint();
			}
			break;
		case MOVE_DIAGRAM:
			if (!this->capturedPoint.isNull()) {
				this->diagram.shift(event->x() - this->capturedPoint.x(), event->y() - this->capturedPoint.y());
				this->capturedPoint = QPoint();
				repaint();
			}
			break;
		default:
			break;
	}

	this->Parent->updateActions();
}

void DiagramWidget::mouseMoveEvent(QMouseEvent *event) {
	if (event->buttons() == 0) {
		switch (this->_editingMode) {
			case NEW_DIAGRAM:
			{
				if (this->diagram.isClosed()) {
					this->setFakeVertex(nullptr);
					break;
				}
				auto fakeVertex = this->fakeVertex;
				if (fakeVertex) {
					fakeVertex->moveTo(event->x(), event->y());
				} else {
					this->setFakeVertex(std::make_shared<KE::TwoD::Diagram::Vertex>(event->x(), event->y()));
				}
				this->repaint();
				break;
			}
			case MOVE_VERTEX:
			case REMOVE_VERTEX:
				this->setCapturedVertex(this->diagram.findVertex(KE::TwoD::FloatPoint(event->x(), event->y()), 17));
				break;
			case ADD_VERTEX:
				this->setCapturedEdge(this->diagram.findEdge(KE::TwoD::FloatPoint(event->x(), event->y()), 5));
				break;
			case FLIP_CROSSING:
				this->setCapturedCrossing(this->diagram.findCrossing(KE::TwoD::FloatPoint(event->x(), event->y()), 17));
				break;
			default:
				break;
		}
	} else {
		switch (this->_editingMode) {
			case ADD_VERTEX:
			case MOVE_VERTEX:
			case NEW_DIAGRAM:
				if (this->capturedVertex) {
					this->diagram.moveVertex(this->capturedVertex, event->x(), event->y());
					repaint();
				}
				return;
			case MOVE_DIAGRAM:
				if (!this->capturedPoint.isNull()) {
					this->diagram.shift(event->x() - this->capturedPoint.x(), event->y() - this->capturedPoint.y());
					this->capturedPoint = event->pos();
					repaint();
				}
				return;
			default:
				return;
		}
	}
}

void DiagramWidget::setFakeVertex(const std::shared_ptr<KE::TwoD::Diagram::Vertex> &vertex) {
	this->fakeVertex = vertex;
	if (vertex) {
		if (this->diagram.vertices().size() <= 1) {
			this->Parent->statusBar()->showMessage("Mouse click adds point");
		} else {
			this->Parent->statusBar()->showMessage("Mouse click adds point; right button click closes the diagram");
		}
	} else {
		this->Parent->statusBar()->clearMessage();
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

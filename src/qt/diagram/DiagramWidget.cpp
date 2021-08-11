#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QStatusBar>

#include "diagramWindow.h"

DiagramWidget::DiagramWidget(diagramWindow *p) : QWidget(p), _editorMode(QUICK_DRAWING) {
	Parent = p;
	this->setMouseTracking(true);
}

DiagramWidget::DiagramWidget(diagramWindow *p, const rapidjson::Document &doc) : QWidget(p), diagram(doc), _editorMode(EDITING) {
	Parent = p;
	this->setMouseTracking(true);
}

bool DiagramWidget::canSetEditorMode(DiagramWidget::EditorMode mode) const {
	switch (mode) {
		default:
		case QUICK_DRAWING:
			return !this->diagram.isClosed();
		case EDITING:
		case MOVING:
			return !this->diagram.vertices().empty();
	}
}

bool DiagramWidget::setEditorMode(DiagramWidget::EditorMode mode) {
	if (mode != this->_editorMode && this->canSetEditorMode(mode)) {
		this->_editorMode = mode;
		this->setFakeVertex(nullptr);
		this->captureVertex(nullptr);
		this->captureEdge(nullptr);
		this->captureCrossing(nullptr);
		return true;
	} else {
		return false;
	}
}

void DiagramWidget::clear() {
	this->diagram.clear();
	this->repaint();
	this->setEditorMode(QUICK_DRAWING);
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

void DiagramWidget::leaveEvent(QEvent*) {
	if (this->fakeVertex) {
		this->setFakeVertex(nullptr);
		this->repaint();
	}
}

void DiagramWidget::mousePressEvent(QMouseEvent *event) {
	switch (this->_editorMode) {
		case QUICK_DRAWING:
		{
			if (this->diagram.isClosed()) {
				this->setFakeVertex(nullptr);
				break;
			}
			auto fakeVertex = this->fakeVertex;
			if (fakeVertex) {
				this->setFakeVertex(nullptr);
				this->captureVertex(this->diagram.addVertex(fakeVertex->x(), fakeVertex->y()), true);
				if (event->button() == Qt::RightButton) {
					this->diagram.close();
					this->setEditorMode(EDITING);
				}
				Parent->isSaved = false;
			}
			break;
		}
		case EDITING:
			if (this->capturedVertex) {
				switch (event->button()) {
					default:
						break;
					case Qt::LeftButton:
						this->Parent->isSaved = false;
						break;
					case Qt::RightButton:
						if (!this->diagram.isClosed() || this->diagram.vertices().size() > 3) {
							this->diagram.removeVertex(this->capturedVertex);
							this->captureVertex(nullptr);
							if (this->diagram.vertices().empty()) {
								this->setEditorMode(QUICK_DRAWING);
							}
							this->repaint();
							this->Parent->isSaved = false;
						}
						break;
				}
			} else if (this->capturedCrossing) {
				this->captureCrossing(this->diagram.flipCrossing(*this->capturedCrossing));
				Parent->isSaved = false;
			} else if (this->capturedEdge) {
				switch (event->button()) {
					default:
						break;
					case Qt::LeftButton:
						this->captureVertex(this->diagram.addVertex(*this->capturedEdge, event->x(), event->y()), true);
						this->captureEdge(nullptr);
						Parent->isSaved = false;
						break;
					case Qt::RightButton:
						// TODO: delete edge
						break;
				}
			}
			break;
		case MOVING:
			this->capturedPoint = event->pos();
			Parent->isSaved = false;
			break;
		default:
			break;
	}
	this->Parent->updateActions();
}

void DiagramWidget::mouseReleaseEvent(QMouseEvent *event) {
	switch (this->_editorMode) {
		case QUICK_DRAWING:
		case EDITING:
			if (this->capturedVertex) {
				this->diagram.moveVertex(this->capturedVertex, event->x(), event->y());
				this->captureVertex(nullptr);
				repaint();
			}
			break;
		case MOVING:
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
		switch (this->_editorMode) {
			case QUICK_DRAWING:
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
			case EDITING:
			{
				auto vertex = this->diagram.findVertex(KE::TwoD::FloatPoint(event->x(), event->y()), 17);
				if (vertex) {
					this->captureEdge(nullptr);
					this->captureCrossing(nullptr);
					this->captureVertex(vertex, false);
					break;
				}

				auto crossing = this->diagram.findCrossing(KE::TwoD::FloatPoint(event->x(), event->y()), 17);
				if (crossing) {
					this->captureVertex(nullptr);
					this->captureEdge(nullptr);
					this->captureCrossing(crossing);
					break;
				}

				this->captureVertex(nullptr);
				this->captureCrossing(nullptr);
				this->captureEdge(this->diagram.findEdge(KE::TwoD::FloatPoint(event->x(), event->y()), 5));
				break;
			}
			default:
				break;
		}
	} else {
		switch (this->_editorMode) {
			case QUICK_DRAWING:
			case EDITING:
				if (this->capturedVertex) {
					this->diagram.moveVertex(this->capturedVertex, event->x(), event->y());
					repaint();
				}
				break;
			case MOVING:
				if (!this->capturedPoint.isNull()) {
					this->diagram.shift(event->x() - this->capturedPoint.x(), event->y() - this->capturedPoint.y());
					this->capturedPoint = event->pos();
					repaint();
				}
				break;
		}
	}
}

void DiagramWidget::setFakeVertex(const std::shared_ptr<KE::TwoD::Diagram::Vertex> &vertex) {
	this->fakeVertex = vertex;

	if (vertex) {
		if (this->diagram.vertices().size() <= 1) {
			this->Parent->statusBar()->showMessage("Mouse click adds point");
		} else {
			this->Parent->statusBar()->showMessage("Left-click adds point; right-click closes the diagram");
		}
	} else {
		this->Parent->statusBar()->clearMessage();
	}
}

void DiagramWidget::captureVertex(const std::shared_ptr<KE::TwoD::Diagram::Vertex> &vertex, bool active) {
	auto old = this->capturedVertex;
	this->capturedVertex = vertex;
	if (old != this->capturedVertex) {
		this->repaint();
	}

	if (vertex && !active) {
		if (this->diagram.isClosed() && this->diagram.vertices().size() <= 3) {
			this->Parent->statusBar()->showMessage("Mouse click starts the point moving");
		} else {
			this->Parent->statusBar()->showMessage("Left-click starts the point moving; right-click deletes the point");
		}
	} else {
		this->Parent->statusBar()->clearMessage();
	}
}

void DiagramWidget::captureEdge(const std::shared_ptr<KE::TwoD::Diagram::Edge> &edge) {
	auto old = this->capturedEdge;
	this->capturedEdge = edge;
	if (old != this->capturedEdge) {
		this->repaint();
	}

	if (edge) {
		this->Parent->statusBar()->showMessage("Mouse click creates point on the edge");
	} else {
		this->Parent->statusBar()->clearMessage();
	}
}

void DiagramWidget::captureCrossing(const std::shared_ptr<KE::TwoD::Diagram::Crossing> &crossing) {
	auto old = this->capturedCrossing;
	this->capturedCrossing = crossing;
	if (old != this->capturedCrossing) {
		this->repaint();
	}

	if (crossing) {
		this->Parent->statusBar()->showMessage("Mouse click flips the crossing");
	} else {
		this->Parent->statusBar()->clearMessage();
	}
}

/*
 * Copyright (c) 1995-2021, Nikolay Pultsin <geometer@geometer.name>
 *
 * Licensed under the Apache License, Version 2.0 the "License";
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <QtGui/QMouseEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QApplication>

#include "DiagramWidget.h"

namespace KE::Qt {

DiagramWidget::DiagramWidget(QWidget *parent) : QWidget(parent), _editorMode(QUICK_DRAWING) {
	this->setMouseTracking(true);
}

DiagramWidget::DiagramWidget(QWidget *parent, const rapidjson::Document &doc) : QWidget(parent), diagram(doc), _editorMode(EDITING) {
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

void DiagramWidget::updateEditorMode() {
	if (this->canSetEditorMode(this->_editorMode)) {
		return;
	}

	if (this->setEditorMode(QUICK_DRAWING) || this->setEditorMode(EDITING)) {
		emit actionsUpdated();
	}
}

void DiagramWidget::clear() {
	this->diagram.clear();
	emit diagramChanged();
	this->repaint();
	this->updateEditorMode();
}

void DiagramWidget::highlightCrossing(QPainter &painter, const TwoD::Diagram::Crossing &crossing) {
	painter.setPen(::Qt::lightGray);
	painter.setBrush(::Qt::lightGray);

	const auto coords = crossing.coords();
	if (coords) {
		painter.drawEllipse(QPointF(coords->x, coords->y), 10, 10);
	}
}

void DiagramWidget::drawVertex(QPainter &painter, const TwoD::Diagram::Vertex &vertex, bool highlight) {
	if (highlight) {
		painter.setPen(::Qt::lightGray);
		painter.setBrush(::Qt::lightGray);
	} else {
		painter.setPen(::Qt::black);
		painter.setBrush(::Qt::black);
	}

	auto coords = vertex.coords();
	painter.drawEllipse(QPointF(coords.x, coords.y), 4.5, 4.5);
}

void DiagramWidget::drawEdge(QPainter &painter, const TwoD::Diagram::Edge &edge, EdgeMode mode) {
	switch (mode) {
		case normal:
			painter.setPen(::Qt::black);
			break;
		case highlighted:
		case fake:
			painter.setPen(::Qt::lightGray);
			break;
		case fake2:
		{
			QPen pen(0xe0e0e0);
			pen.setDashPattern({6.0, 10.0});
			painter.setPen(pen);
			break;
		}
	}

	const auto start = edge.start->coords();
	const auto end = edge.end->coords();
	float deltaX = end.x - start.x;
	float deltaY = end.y - start.y;
	float hyp = hypotf(deltaX, deltaY);

	deltaX = 10 * deltaX / hyp;
	deltaY = 10 * deltaY / hyp;

	float x0 = start.x,
				y0 = start.y,
				x1, y1;

	for (const auto &crs : this->diagram.underCrossings(edge)) {
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

	x1 = end.x;
	y1 = end.y;

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
			const TwoD::Diagram::Edge edge(fakeVertex, vertices.back());
			drawEdge(painter, edge, EdgeMode::fake);
			if (vertices.size() > 1) {
				const TwoD::Diagram::Edge edge2(fakeVertex, vertices.front());
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
	pnt.fillRect(0, 0, this->width(), this->height(), ::Qt::white);
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
				const auto coords = fakeVertex->coords();
				this->captureVertex(this->diagram.addVertex(coords.x, coords.y), true);
				if (event->button() == ::Qt::RightButton) {
					this->diagram.close();
					this->updateEditorMode();
				}
				emit diagramChanged();
			}
			break;
		}
		case EDITING:
			if (this->capturedVertex) {
				switch (event->button()) {
					default:
						break;
					case ::Qt::LeftButton:
						break;
					case ::Qt::RightButton:
						if (this->diagram.canRemoveVertex(this->capturedVertex)) {
							this->diagram.removeVertex(this->capturedVertex);
							this->captureVertex(nullptr);
							this->updateEditorMode();
							emit diagramChanged();
						}
						break;
				}
			} else if (this->capturedCrossing) {
				this->captureCrossing(this->diagram.flipCrossing(*this->capturedCrossing));
				emit diagramChanged();
			} else if (this->capturedEdge) {
				switch (event->button()) {
					default:
						break;
					case ::Qt::LeftButton:
						this->captureVertex(this->diagram.addVertex(*this->capturedEdge, event->pos().x(), event->pos().y()), true);
						this->captureEdge(nullptr);
						emit diagramChanged();
						break;
					case ::Qt::RightButton:
						if (this->diagram.canRemoveEdge(this->capturedEdge)) {
							this->diagram.removeEdge(this->capturedEdge);
							this->captureEdge(nullptr);
							this->updateEditorMode();
							emit diagramChanged();
						}
						break;
				}
			}
			break;
		case MOVING:
			this->capturePoint(event->pos());
			break;
		default:
			break;
	}
	emit actionsUpdated();
	this->selectMouseCursor();
}

void DiagramWidget::mouseReleaseEvent(QMouseEvent *event) {
	switch (this->_editorMode) {
		case QUICK_DRAWING:
		case EDITING:
			if (this->capturedVertex) {
				this->diagram.moveVertex(this->capturedVertex, event->pos().x(), event->pos().y(), true);
				this->captureVertex(nullptr);
				emit diagramChanged();
				this->repaint();
			}
			break;
		case MOVING:
			if (!this->capturedPoint.isNull()) {
				this->diagram.shift(event->pos().x() - this->capturedPoint.x(), event->pos().y() - this->capturedPoint.y(), true);
				this->capturePoint(QPoint());
				emit diagramChanged();
				this->repaint();
			}
			break;
		default:
			break;
	}

	emit actionsUpdated();
	this->diagram.savePoint();
	this->selectMouseCursor();
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
					fakeVertex->moveTo(event->pos().x(), event->pos().y());
				} else {
					this->setFakeVertex(std::make_shared<TwoD::Diagram::Vertex>(
						event->pos().x(),
						event->pos().y(),
						std::numeric_limits<std::size_t>::max()
					));
				}
				this->repaint();
				break;
			}
			case EDITING:
			{
				auto vertex = this->diagram.findVertex(TwoD::FloatPoint(event->pos().x(), event->pos().y()), 17);
				if (vertex) {
					this->captureEdge(nullptr);
					this->captureCrossing(nullptr);
					this->captureVertex(vertex, false);
					break;
				}

				auto crossing = this->diagram.findCrossing(TwoD::FloatPoint(event->pos().x(), event->pos().y()), 17);
				if (crossing) {
					this->captureVertex(nullptr);
					this->captureEdge(nullptr);
					this->captureCrossing(crossing);
					break;
				}

				this->captureVertex(nullptr);
				this->captureCrossing(nullptr);
				this->captureEdge(this->diagram.findEdge(TwoD::FloatPoint(event->pos().x(), event->pos().y()), 5));
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
					this->diagram.moveVertex(this->capturedVertex, event->pos().x(), event->pos().y(), false);
					emit diagramChanged();
					this->repaint();
				}
				break;
			case MOVING:
				if (!this->capturedPoint.isNull()) {
					this->diagram.shift(event->pos().x() - this->capturedPoint.x(), event->pos().y() - this->capturedPoint.y(), false);
					this->capturePoint(event->pos());
					emit diagramChanged();
					this->repaint();
				}
				break;
		}
	}
}

void DiagramWidget::setFakeVertex(const std::shared_ptr<TwoD::Diagram::Vertex> &vertex) {
	if (vertex != this->fakeVertex) {
		this->fakeVertex = vertex;
		this->selectMouseCursor();
	}

	if (vertex) {
		if (this->diagram.vertices().size() <= 1) {
			emit setActionTip("Mouse click adds point");
		} else {
			emit setActionTip("Left-click adds point; right-click closes the diagram");
		}
	} else {
		emit setActionTip(QString());
	}
}

void DiagramWidget::captureVertex(const std::shared_ptr<TwoD::Diagram::Vertex> &vertex, bool active) {
	auto old = this->capturedVertex;
	this->capturedVertex = vertex;
	if (old != this->capturedVertex) {
		this->selectMouseCursor();
		this->repaint();
	}

	if (vertex && !active) {
		if (!this->diagram.canRemoveVertex(vertex)) {
			emit setActionTip("Mouse click starts the point moving");
		} else {
			emit setActionTip("Left-click starts the point moving; right-click deletes the point");
		}
	} else {
		emit setActionTip(QString());
	}
}

void DiagramWidget::captureEdge(const std::shared_ptr<TwoD::Diagram::Edge> &edge) {
	auto old = this->capturedEdge;
	this->capturedEdge = edge;
	if (old != this->capturedEdge) {
		this->selectMouseCursor();
		this->repaint();
	}

	if (edge) {
		if (this->diagram.canRemoveEdge(edge)) {
			emit setActionTip("Left-click creates point on the edge; right-click deletes the edge");
		} else {
			emit setActionTip("Mouse click creates point on the edge");
		}
	} else {
		emit setActionTip(QString());
	}
}

void DiagramWidget::captureCrossing(const std::shared_ptr<TwoD::Diagram::Crossing> &crossing) {
	auto old = this->capturedCrossing;
	this->capturedCrossing = crossing;
	if (old != this->capturedCrossing) {
		this->selectMouseCursor();
		this->repaint();
	}

	if (crossing) {
		emit setActionTip("Mouse click flips the crossing");
	} else {
		emit setActionTip(QString());
	}
}

void DiagramWidget::capturePoint(const QPoint &point) {
	this->capturedPoint = point;
	this->selectMouseCursor();
}

void DiagramWidget::selectMouseCursor() {
	if (this->capturedVertex || this->capturedEdge || this->fakeVertex) {
		if (QApplication::mouseButtons() == ::Qt::NoButton) {
			this->setCursor(::Qt::PointingHandCursor);
		} else {
			this->setCursor(::Qt::DragMoveCursor);
		}
	} else if (this->capturedCrossing) {
		this->setCursor(::Qt::PointingHandCursor);
	} else if (!this->capturedPoint.isNull()) {
		this->setCursor(::Qt::DragMoveCursor);
	} else {
		this->unsetCursor();
	}
}

}

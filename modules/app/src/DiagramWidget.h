/*
 * Copyright (c) 1995-2021, Mathesis Software <mad@mathesis.fun>
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

#ifndef __DIAGRAMWIDGET_H__
#define __DIAGRAMWIDGET_H__

#include <QtWidgets/QWidget>

#include <DiagramEditor.h>

namespace KE::Qt {

class DiagramWidget : public QWidget {

Q_OBJECT

public:
	enum EditorMode {
		QUICK_DRAWING = 0,
		EDITING = 1,
		MOVING = 2
	};

public:
	TwoD::DiagramEditor diagram;

private:
	DiagramWidget::EditorMode _editorMode;

	std::shared_ptr<TwoD::Diagram::Vertex> fakeVertex;
	std::shared_ptr<TwoD::Diagram::Vertex> capturedVertex;
	std::shared_ptr<TwoD::Diagram::Edge> capturedEdge;
	std::shared_ptr<TwoD::Diagram::Crossing> capturedCrossing;
	QPoint capturedPoint;

	void setFakeVertex(const std::shared_ptr<TwoD::Diagram::Vertex> &vertex);
	void captureVertex(const std::shared_ptr<TwoD::Diagram::Vertex> &vertex, bool active = false);
	void captureEdge(const std::shared_ptr<TwoD::Diagram::Edge> &edge);
	void captureCrossing(const std::shared_ptr<TwoD::Diagram::Crossing> &crossing);
	void capturePoint(const QPoint &point);

	void drawVertex(QPainter &, const TwoD::Diagram::Vertex &vertex, bool highlight);
	enum EdgeMode {
		normal,
		highlighted,
		fake,
		fake2
	};
	void drawEdge(QPainter &, const TwoD::Diagram::Edge &edge, EdgeMode mode);
	void highlightCrossing(QPainter &painter, const TwoD::Diagram::Crossing &crossing);

	void paintEvent(QPaintEvent*) override;
	void leaveEvent(QEvent*) override;
	void mousePressEvent(QMouseEvent*) override;
	void mouseReleaseEvent(QMouseEvent*) override;
	void mouseMoveEvent(QMouseEvent*) override;

	void selectMouseCursor();

public:
	DiagramWidget(QWidget *parent, const rapidjson::Value &doc);
	DiagramWidget(QWidget *parent);

	void drawIt(QPainter &painter);

	EditorMode editorMode() const { return this->_editorMode; }
	bool canSetEditorMode(DiagramWidget::EditorMode mode) const;
	bool setEditorMode(DiagramWidget::EditorMode mode);
	void updateEditorMode();

	void clear();

signals:
	void setActionTip(const QString &tip);
	void actionsUpdated();
	void diagramChanged();
};

}

#endif /* __DIAGRAMWIDGET_H__ */

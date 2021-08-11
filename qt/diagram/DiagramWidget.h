#ifndef __DIAGRAMWIDGET_H__
#define __DIAGRAMWIDGET_H__

#include <QtWidgets/QWidget>

#include "../../math/diagram/diagram.h"

class diagramWindow;

class DiagramWidget : public QWidget {

public:
	enum EditorMode {
		QUICK_DRAWING = 0,
		EDITING = 1,
		MOVING = 2
	};

public:
	KE::TwoD::Diagram diagram;

private:
	diagramWindow *Parent;
	DiagramWidget::EditorMode _editorMode;

	QPoint capturedPoint;
	std::shared_ptr<KE::TwoD::Diagram::Vertex> fakeVertex;
	std::shared_ptr<KE::TwoD::Diagram::Vertex> capturedVertex;
	std::shared_ptr<KE::TwoD::Diagram::Edge> capturedEdge;
	std::shared_ptr<KE::TwoD::Diagram::Crossing> capturedCrossing;

	void setFakeVertex(const std::shared_ptr<KE::TwoD::Diagram::Vertex> &vertex);
	void captureVertex(const std::shared_ptr<KE::TwoD::Diagram::Vertex> &vertex);
	void captureEdge(const std::shared_ptr<KE::TwoD::Diagram::Edge> &edge);
	void captureCrossing(const std::shared_ptr<KE::TwoD::Diagram::Crossing> &crossing);

	void drawVertex(QPainter&, const KE::TwoD::Diagram::Vertex &vertex, bool highlight);
	enum EdgeMode {
		normal,
		highlighted,
		fake,
		fake2
	};
	void drawEdge(QPainter&, const KE::TwoD::Diagram::Edge &edge, EdgeMode mode);
	void highlightCrossing(QPainter &painter, const KE::TwoD::Diagram::Crossing &crossing);

	void paintEvent(QPaintEvent*) override;
	void leaveEvent(QEvent*) override;
	void mousePressEvent(QMouseEvent*) override;
	void mouseReleaseEvent(QMouseEvent*) override;
	void mouseMoveEvent(QMouseEvent*) override;

public:
	DiagramWidget(diagramWindow *p, const rapidjson::Document &doc);
	DiagramWidget(diagramWindow *p);

	void drawIt(QPainter &painter);

	EditorMode editorMode() const { return this->_editorMode; }
	bool canSetEditorMode(DiagramWidget::EditorMode mode) const;
	bool setEditorMode(DiagramWidget::EditorMode mode);

	void clear();
};

#endif /* __DIAGRAMWIDGET_H__ */

#ifndef __DIAGRAMWIDGET_H__
#define __DIAGRAMWIDGET_H__

#include <QtWidgets/QWidget>

#include "../../math/diagramEditor/DiagramEditor.h"

class DiagramWidget : public QWidget {

Q_OBJECT

public:
	enum EditorMode {
		QUICK_DRAWING = 0,
		EDITING = 1,
		MOVING = 2
	};

public:
	KE::TwoD::DiagramEditor diagram;

private:
	DiagramWidget::EditorMode _editorMode;

	std::shared_ptr<KE::TwoD::Diagram::Vertex> fakeVertex;
	std::shared_ptr<KE::TwoD::Diagram::Vertex> capturedVertex;
	std::shared_ptr<KE::TwoD::Diagram::Edge> capturedEdge;
	std::shared_ptr<KE::TwoD::Diagram::Crossing> capturedCrossing;
	QPoint capturedPoint;

	void setFakeVertex(const std::shared_ptr<KE::TwoD::Diagram::Vertex> &vertex);
	void captureVertex(const std::shared_ptr<KE::TwoD::Diagram::Vertex> &vertex, bool active = false);
	void captureEdge(const std::shared_ptr<KE::TwoD::Diagram::Edge> &edge);
	void captureCrossing(const std::shared_ptr<KE::TwoD::Diagram::Crossing> &crossing);
	void capturePoint(const QPoint &point);

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

	void selectMouseCursor();

public:
	DiagramWidget(QWidget *parent, const rapidjson::Document &doc);
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
};

#endif /* __DIAGRAMWIDGET_H__ */

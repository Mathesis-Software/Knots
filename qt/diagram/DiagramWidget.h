#ifndef __DIAGRAMWIDGET_H__
#define __DIAGRAMWIDGET_H__

#include <QtWidgets/QWidget>

#include "../../math/diagram/diagram.h"

class diagramWindow;

class DiagramWidget : public QWidget {

public:
	enum EditingMode {
		NEW_DIAGRAM = 0,
		ADD_VERTEX = 1,
		MOVE_VERTEX = 2,
		REMOVE_VERTEX = 3,
		FLIP_CROSSING = 4,
		MOVE_DIAGRAM = 5
	};

public:
	KE::TwoD::Diagram diagram;

private:
	diagramWindow *Parent;
	DiagramWidget::EditingMode _editingMode;

	QPoint capturedPoint;
	std::shared_ptr<KE::TwoD::Diagram::Vertex> fakeVertex;
	std::shared_ptr<KE::TwoD::Diagram::Vertex> capturedVertex;
	std::shared_ptr<KE::TwoD::Diagram::Edge> capturedEdge;
	std::shared_ptr<KE::TwoD::Diagram::Crossing> capturedCrossing;

	void setFakeVertex(const std::shared_ptr<KE::TwoD::Diagram::Vertex> &vertex);
	void setCapturedVertex(const std::shared_ptr<KE::TwoD::Diagram::Vertex> &vertex);
	void setCapturedEdge(const std::shared_ptr<KE::TwoD::Diagram::Edge> &edge);
	void setCapturedCrossing(const std::shared_ptr<KE::TwoD::Diagram::Crossing> &crossing);

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

	EditingMode editingMode() const { return this->_editingMode; }
	bool canSetEditingMode(DiagramWidget::EditingMode mode) const;
	bool setEditingMode(DiagramWidget::EditingMode mode);

	void clear();
};

#endif /* __DIAGRAMWIDGET_H__ */

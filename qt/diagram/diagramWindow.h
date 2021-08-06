#ifndef __DIAGRAMWINDOW_H__
#define __DIAGRAMWINDOW_H__

#include <QtWidgets/QMenu>
#include <QtWidgets/QToolButton>

#include "../abstractWindow/abstractWindow.h"
#include "../../math/diagram/diagram.h"

class diagramWindow;

class DiagramWidget : public QWidget {

	Q_OBJECT

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

	void drawVertex(QPainter&, const KE::TwoD::Diagram::Vertex &vertex);
	void drawEdge(QPainter&, const KE::TwoD::Diagram::Edge &edge);

	void paintEvent(QPaintEvent*) override;
	void mousePressEvent(QMouseEvent*) override;
	void mouseReleaseEvent(QMouseEvent*) override;
	void mouseMoveEvent(QMouseEvent*) override;

public:
	DiagramWidget(diagramWindow *p, const rapidjson::Document &doc);
	DiagramWidget(diagramWindow *p);

	void drawIt(QPainter &painter);
};

class diagramWindow : public abstractWindow {

Q_OBJECT

private:
	QMenu *actionsMenu;
	QAction *actions_convert;
	QAction *actions_simplify;
	QAction *actions_clear;

	QToolButton **actions;

	DiagramWidget::EditingMode mode;

	void init(DiagramWidget *widget);

	const char *mask() const override {return "*.dgr";};

	void printIt(QPrinter*);
	void saveIt(std::ostream&);

	void setMode(DiagramWidget::EditingMode mode);
	
private slots:
	void convert();
	void clear();
	void simplify();

public:
	diagramWindow(const rapidjson::Document &doc);
	diagramWindow();
	~diagramWindow();

	DiagramWidget *diagramWidget() const { return (DiagramWidget*)this->centralWidget(); }
	bool isEmpty() const override;

	friend class DiagramWidget;
};

#endif /* __DIAGRAMWINDOW_H__ */

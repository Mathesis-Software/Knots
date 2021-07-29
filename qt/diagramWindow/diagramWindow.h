#ifndef __DIAGRAMWINDOW_H__
#define __DIAGRAMWINDOW_H__

#include <memory>

#include <QtWidgets/QMenu>
#include <QtWidgets/QToolButton>

#include "../abstractWindow/abstractWindow.h"
#include "../../math/diagram/diagram.h"

class diagramWindow : public abstractWindow {

Q_OBJECT

private:
  enum editorMode {
    DRAW_NEW_DIAGRAM = 0,
    ADD_POINT = 1,
    MOVE_POINT = 2,
    REMOVE_POINT = 3,
    CHANGE_CROSS = 4,
    MOVE_DIAGRAM = 5,
    editorModeNumber = 6
  };

public:
	std::shared_ptr<KE::TwoD::Diagram> diagram;

private:
  QMenu *actionsMenu;
  QAction *actions_convert;
  QAction *actions_simplify;
  QAction *actions_clear;

  QToolButton **actions;

  bool isClosed;
  editorMode mode;

  void init();

  const char *mask() {return "*.dgr";};

  void printIt(QPrinter*);
  void saveIt(std::ostream&);

  vertex *nearToVertex(int x, int y);
  crossing *nearToCross(int x, int y);
  vertex *nearToEdge(int x, int y);
  
private slots:
  void convert();
  void clear();
  void setmode(int);
  void simplify();

public:
  diagramWindow(std::istream&);
  diagramWindow();
  ~diagramWindow();

  bool isEmpty() { return this->diagram->isEmpty(); }

  friend class diagramMainWidget;
};

class diagramMainWidget : public QWidget {

  Q_OBJECT

private:

  diagramWindow *Parent;

  void drawPoint(QPainter*, vertex*);
  void drawEdge(QPainter*, vertex*);

  void paintEvent(QPaintEvent*);
  void mousePressEvent(QMouseEvent*);
  void mouseReleaseEvent(QMouseEvent*);
  void mouseMoveEvent(QMouseEvent*);

public:
  diagramMainWidget(diagramWindow *p);

  void drawIt(QPainter*);
};

#endif /* __DIAGRAMWINDOW_H__ */

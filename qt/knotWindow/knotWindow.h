#ifndef __KNOTWINDOW_H__
#define __KNOTWINDOW_H__

#include <QtOpenGL/qgl.h>

#include "../diagramWindow/diagramWindow.h"
#include "../3dWindow/3dWindow.h"
#include "../../gl/surface/surface.h"
#include "../../math/knot/knot.h"
#include "../../math/seifert/seifert.h"

class paramWindow;

class knotWindow : public dddWindow, public Knot {

  Q_OBJECT

private:
  surface *kSurf, *sSurf;
  double thickness;
  double bp [3];

  QMenu *mathMenu;
  QMenu *viewMenu;
  QAction *math_decreaseEnergy;
  QAction *math_stop;
  QAction *view_showKnot;
  QAction *view_showSeifertSurface;

  friend class paramWindow;
  paramWindow *mth;

  void init();
  void initMenu();

  bool smoothing;
  bool continuousSmoothing;
  int smoothSteps;
  int redrawAfter;
  void startSmooth (int, int, bool = true);

  int timerId_smooth;
  void doSmooth();
  void timerEvent (QTimerEvent*);

  bool isEmpty() {return Knot::isEmpty();};

  void readIt (std::istream&);
  void saveIt (std::ostream&);

private slots:
  void stop();
  void math();
  void smooth();
  void setLength();
  void setNumberOfPoints();
  void decreaseEnergy();
  void switchShowKnot();
  void switchShowSeifert();
  void setThickness();
  void setBgColor();
  void setKnotColor();
  void setSeifertFrontColor();
  void setSeifertBackColor();
  void bp_plus();
  void bp_minus();

public:
  knotWindow (std::istream&);
  knotWindow (diagramWindow*);
  ~knotWindow();

  const char *mask() {return "*.knt";};
};

#endif /* __KNOTWINDOW_H__ */

#ifndef __KNOTWINDOW_H__
#define __KNOTWINDOW_H__

#include <QtCore/QThread>

#include "../gl/GLWindow.h"
#include "../../gl/surface/surface.h"
#include "../../math/knot/Knot.h"
#include "../../math/seifert/seifert.h"

namespace KE { namespace GL {

class KnotSurface;
class SeifertSurface;

}}

class paramWindow;
class diagramWindow;
class knotWindow;

class SmoothingThread : public QThread {

Q_OBJECT

private:
	knotWindow &knot;

public:
	SmoothingThread(knotWindow &knot);

signals:
	void knotChanged();

private:
	void run() override;
};

class knotWindow : public GLWindow {

friend class SmoothingThread;

Q_OBJECT

private:
	KE::ThreeD::Knot knot;

	std::shared_ptr<KE::GL::KnotSurface> knotSurface;
	KE::ThreeD::Point seifertStartPoint;
	std::shared_ptr<KE::GL::SeifertSurface> seifertSurface;
  double thickness;
	SmoothingThread smoothingThread;

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

  bool continuousSmoothing;
  int smoothSteps;
  int redrawAfter;
  void startSmooth(int, int, bool = true);

  void doSmooth();
	void onKnotChanged();

  bool isEmpty() const override { return this->knot.points().size() == 0; }

  void saveIt(std::ostream&);

private:
	void runColorDialog(const QString &title, std::function<QColor()> getter, std::function<void(const QColor&)> setter);

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
  knotWindow(const rapidjson::Document &doc);
  knotWindow(const diagramWindow &diagram);
  ~knotWindow();

	void closeEvent(QCloseEvent *event);

private:
  const char *mask() const override {return "*.knt";};
	void updateActions() override;
	void rename() override;
};

#endif /* __KNOTWINDOW_H__ */

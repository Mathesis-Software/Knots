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
	bool _isSaved;

	std::shared_ptr<KE::GL::KnotSurface> knotSurface;
	KE::ThreeD::Point seifertStartPoint;
	std::shared_ptr<KE::GL::SeifertSurface> seifertSurface;
  double thickness;
	SmoothingThread smoothingThread;

  QMenu *mathMenu;
  QMenu *viewMenu;

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

	bool isSaved() const override { return this->_isSaved; }

  void saveIt(std::ostream&);

private:
	void runColorDialog(const QString &title, std::function<QColor()> getter, std::function<void(const QColor&)> setter);

  void toggleKnotVisibility();
  void toggleSeifertSurfaceVisibility();

private slots:
  void stop();
  void math();
  void smooth();
  void setLength();
  void setNumberOfPoints();
  void decreaseEnergy();
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
	QString fileFilter() const override { return "Knot files (*.knt)"; }
	void updateActions() override;
	void rename() override;
};

#endif /* __KNOTWINDOW_H__ */

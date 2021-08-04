#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPixmap>
#include <QtCore/QTimerEvent>

#include "knotWindow.h"
#include "knotWindow_math.h"
#include "../diagramWindow/diagramWindow.h"
#include "../../math/knot/knot_surface.h"
#include "../../math/seifert/seifert_surface.h"

void knotWindow::init() {
  this->knotSurface = std::make_shared<KE::GL::KnotSurface>(this->knot, 0.05);
  addSurface(this->knotSurface);
  this->seifertSurface = std::make_shared<KE::GL::SeifertSurface>(this->knot, this->seifertStartPoint);
  addSurface(this->seifertSurface);

  float rgb[3];
  rgb[0] = 1.0;
  rgb[1] = 1.0;
  rgb[2] = 1.0;
  this->knotSurface->setFrontRGB(rgb);
  rgb[0] = 1.0;
  rgb[1] = 1.0;
  rgb[2] = 1.0;
  this->seifertSurface->setFrontRGB(rgb);
  rgb[0] = 0.5;
  rgb[1] = 0.5;
  rgb[2] = 0.5;
  this->seifertSurface->setBackRGB(rgb);

  this->knotSurface->show();

  thickness = 1.0;
  smoothing = false;
  mth = NULL;

  initMenu();
  complete();

  setWindowTitle(this->knot.caption.c_str());
  setWindowIcon(QPixmap((QString)getenv("KNOTEDITOR_PIXMAPS") + "/trefoil.xpm"));
}

knotWindow::knotWindow(const rapidjson::Document &doc) : knot(doc), seifertStartPoint(0.0, 0.0, 0.4) {
  this->init();
}

knotWindow::knotWindow(const diagramWindow &d) : knot(d.diagram, d.width(), d.height()), seifertStartPoint(0.0, 0.0, 0.4) {
  this->init();
	this->isSaved = false;
}

knotWindow::~knotWindow() {
  if (mth)
    delete mth;

  delete mathMenu;
  delete viewMenu;
}

void knotWindow::timerEvent(QTimerEvent *te) {
  if (smoothing && (te->timerId() == timerId_smooth)) {
    doSmooth();
	} else {
    dddWindow::timerEvent (te);
	}
}

void knotWindow::switchShowKnot() {
  if (this->knotSurface->isVisible()) {
    this->knotSurface->hide();
    view_showKnot->setChecked(false);
  } else {
    this->knotSurface->show();
    view_showKnot->setChecked(true);
  }

  repaint3d ();
}

void knotWindow::switchShowSeifert ()
{
  if (this->seifertSurface->isVisible()) {
    this->seifertSurface->hide ();
    view_showSeifertSurface->setChecked(false);
  } else {
    this->seifertSurface->show ();
    view_showSeifertSurface->setChecked(true);
  }

  repaint3d ();
}

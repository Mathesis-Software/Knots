#include <QtCore/QTimerEvent>
#include <QtGui/QCloseEvent>
#include <QtGui/QPixmap>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>

#include "knotWindow.h"
#include "knotWindow_math.h"
#include "../diagram/diagramWindow.h"
#include "../../math/knot/KnotSurface.h"
#include "../../math/seifert/seifert_surface.h"

void knotWindow::init() {
  this->knotSurface = std::make_shared<KE::GL::KnotSurface>(this->knot, 0.05, 28);
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
  mth = NULL;

  initMenu();
  complete();

  setWindowIcon(QPixmap((QString)getenv("KNOTEDITOR_PIXMAPS") + "/trefoil.xpm"));
	this->updateActions();
}

knotWindow::knotWindow(const rapidjson::Document &doc) : knot(doc), seifertStartPoint(0.0, 0.0, 0.4), smoothingThread(*this) {
  this->init();
}

knotWindow::knotWindow(const diagramWindow &d) : knot(d.diagramWidget()->diagram.diagram(), d.width(), d.height()), seifertStartPoint(0.0, 0.0, 0.4), smoothingThread(*this) {
  this->init();
	this->isSaved = false;
}

knotWindow::~knotWindow() {
  if (mth) {
    delete mth;
	}

  delete mathMenu;
  delete viewMenu;
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

void knotWindow::switchShowSeifert() {
  if (this->seifertSurface->isVisible()) {
    this->seifertSurface->hide ();
    view_showSeifertSurface->setChecked(false);
  } else {
    this->seifertSurface->show ();
    view_showSeifertSurface->setChecked(true);
  }

  repaint3d ();
}

void knotWindow::closeEvent(QCloseEvent *event) {
	abstractWindow::closeEvent(event);
	if (event->isAccepted() && this->smoothingThread.isRunning()) {
		this->smoothingThread.requestInterruption();
		this->smoothingThread.wait();
	}
}

void knotWindow::updateActions() {
  setWindowTitle(this->knot.caption.c_str());
	abstractWindow::updateActions();
}

void knotWindow::rename() {
	bool ok;
	const QString text = QInputDialog::getText(
		this, "Rename knot", "New knot name:", QLineEdit::Normal, this->knot.caption.c_str(), &ok
	);
  if (ok) {
		this->knot.caption = text.toStdString();
		this->isSaved = false;
		this->updateActions();
	}
}
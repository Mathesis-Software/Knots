#include <QtGui/QCloseEvent>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>

#include "knotWindow.h"
#include "knotWindow_math.h"
#include "../gl/GLWidget.h"
#include "../diagram/diagramWindow.h"
#include "../../math/knotSurface/KnotSurface.h"
#include "../../math/seifert/SeifertSurface.h"

namespace {

class KnotWidget : public GLWidget {

private:
	const KE::ThreeD::KnotWrapper &knot;

public:
	KnotWidget(QWidget *parent, const KE::ThreeD::KnotWrapper &knot) : GLWidget(parent), knot(knot) {}
	const KE::GL::Color &backgroundColor() const override {
		const auto ref = this->knot.backgroundColor;
		return ref ? *ref : KE::GL::Color::white;
	}
};

}

void knotWindow::init() {
	this->setCentralWidget(new KnotWidget(this, this->knot));
  this->knotSurface = std::make_shared<KE::GL::KnotSurface>(this->knot, 28);
  this->knotWidget()->addSurface(this->knotSurface);
  this->seifertSurface = std::make_shared<KE::GL::SeifertSurface>(this->knot, this->seifertStartPoint);
  this->knotWidget()->addSurface(this->seifertSurface);

  mth = NULL;

  initMenu();
  complete();

	this->updateActions();
}

knotWindow::knotWindow(const rapidjson::Document &doc) : knot(doc), seifertStartPoint(0.0, 0.0, 0.4), smoothingThread(*this) {
  this->init();
}

knotWindow::knotWindow(const diagramWindow &d) : knot(d.diagramWidget()->diagram.diagram(), d.width(), d.height()), seifertStartPoint(0.0, 0.0, 0.4), smoothingThread(*this) {
  this->init();
}

knotWindow::~knotWindow() {
  if (mth) {
    delete mth;
	}

  delete mathMenu;
  delete viewMenu;
}

void knotWindow::toggleSeifertSurfaceVisibility() {
  if (this->seifertSurface->isVisible()) {
    this->knot.isSeifertSurfaceVisible = std::make_shared<bool>(false);
  } else {
    this->knot.isSeifertSurfaceVisible = std::make_shared<bool>(true);
  }
	this->seifertSurface->destroy(true);

  this->centralWidget()->update();
	this->updateActions();
}

void knotWindow::closeEvent(QCloseEvent *event) {
	abstractWindow::closeEvent(event);
	if (event->isAccepted() && this->smoothingThread.isRunning()) {
		this->smoothingThread.requestInterruption();
		this->smoothingThread.wait();
	}
}

void knotWindow::updateActions() {
  setWindowTitle(this->knot.caption().c_str());
	abstractWindow::updateActions();
}

void knotWindow::rename() {
	bool ok;
	const QString text = QInputDialog::getText(
		this, "Rename knot", "New knot name:", QLineEdit::Normal, this->knot.caption().c_str(), &ok
	);
  if (ok) {
		this->knot.setCaption(text.toStdString());
		this->updateActions();
	}
}

void knotWindow::moveSeifertBasePoint(double distance) {
	this->seifertStartPoint.move(
		KE::GL::SeifertSurface::gradient(this->seifertStartPoint, this->knot.snapshot()), distance
	);
	this->seifertSurface->destroy(true);
	this->centralWidget()->update();
}

GLWidget *knotWindow::knotWidget() const {
	return (GLWidget*)this->centralWidget();
}

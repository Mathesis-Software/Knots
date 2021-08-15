#include <QtWidgets/QMenuBar>

#include "knotWindow.h"

#include "../../math/knotSurface/KnotSurface.h"
#include "../../math/seifert/seifert_surface.h"

void knotWindow::initMenu() {
  mathMenu = menuBar()->addMenu("Math");
  mathMenu->addAction("View parameters", this, SLOT(math()));
  mathMenu->addSeparator();
	this->registerAction(
  	mathMenu->addAction("Start smoothing…", this, SLOT(decreaseEnergy())),
		[this](QAction &action) { action.setEnabled(!this->smoothingThread.isRunning()); }
	);
	this->registerAction(
  	mathMenu->addAction("Stop smoothing", this, SLOT(stop())),
		[this](QAction &action) { action.setEnabled(this->smoothingThread.isRunning()); }
	);
  mathMenu->addSeparator();
  mathMenu->addAction("Number of points…", this, SLOT(setNumberOfPoints()));
  mathMenu->addAction("Length…", this, SLOT(setLength()));
  
  viewMenu = menuBar()->addMenu("View");
	auto toggleKnot = this->registerAction(
		viewMenu->addAction("Show knot", this, &knotWindow::toggleKnotVisibility),
		[this](QAction &action) {
			action.setChecked(this->knotSurface->isVisible());  
		}
	);
	toggleKnot->setCheckable(true);
	auto toggleSeifert = this->registerAction(
		viewMenu->addAction("Show Seifert surface", this, &knotWindow::toggleSeifertSurfaceVisibility),
		[this](QAction &action) {
			action.setChecked(this->seifertSurface->isVisible());  
		}
	);
	toggleSeifert->setCheckable(true);
  
  QMenu *optionsMenu = menuBar()->addMenu("Options");
  optionsMenu->addAction("Thickness…", this, SLOT(setThickness()));
  optionsMenu->addSeparator();
  optionsMenu->addAction("Background color…", this, SLOT(setBgColor()));
  optionsMenu->addAction("Knot color…", this, SLOT(setKnotColor()));
  optionsMenu->addAction("Seifert surface front color…", this, SLOT(setSeifertFrontColor()));
  optionsMenu->addAction("Seifert surface back color…", this, SLOT(setSeifertBackColor()));

	this->registerAction(
		addToolbarAction("smooth.svg", "Start smoothing", [this] { this->smooth(); }),
		[this](QAction &action) { action.setVisible(!this->smoothingThread.isRunning()); }
	);
	this->registerAction(
		addToolbarAction("stop.svg", "Interrupt smoothing", [this] { this->stop(); }),
		[this](QAction &action) { action.setVisible(this->smoothingThread.isRunning()); }
	);
  addToolbarSeparator();
  addToolbarAction("math.svg", "Show parameters", [this] { this->math(); });
  addToolbarSeparator();
  this->registerAction(
		addToolbarAction("plus.svg", "Shift Seifert surface forward along the gradient", [this] { this->bp_plus(); }),
		[this](QAction &action) { action.setEnabled(this->seifertSurface->isVisible()); }
	);
  this->registerAction(
  	addToolbarAction("minus.svg", "Shift Seifert surface back along the gradient", [this] { this->bp_minus(); }),
		[this](QAction &action) { action.setEnabled(this->seifertSurface->isVisible()); }
	);

	this->updateActions();
}

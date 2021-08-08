#include <QtWidgets/QMenuBar>

#include "knotWindow.h"

#include "../../math/knot/knot_surface.h"
#include "../../math/seifert/seifert_surface.h"

void knotWindow::initMenu() {
  mathMenu = menuBar()->addMenu("&Math");
  mathMenu->addAction("&View parameters", this, SLOT(math()));
  mathMenu->addSeparator();
  math_decreaseEnergy = mathMenu->addAction("Decrease &energy…", this, SLOT(decreaseEnergy()));
  math_decreaseEnergy->setEnabled(!smoothing);
  math_stop = mathMenu->addAction("&Stop", this, SLOT(stop()));
  math_stop->setEnabled(smoothing);
  mathMenu->addSeparator();
  mathMenu->addAction("Number of &points…", this, SLOT(setNumberOfPoints()));
  mathMenu->addAction("&Length…", this, SLOT(setLength()));
  
  viewMenu = menuBar()->addMenu("&View");
  view_showKnot = viewMenu->addAction("Show &knot", this, SLOT(switchShowKnot()));
  view_showKnot->setCheckable(true);
  view_showKnot->setChecked(this->knotSurface->isVisible());  
  view_showSeifertSurface = viewMenu->addAction("Show &Seifert surface", this, SLOT(switchShowSeifert()));
  view_showSeifertSurface->setCheckable(true);
  view_showSeifertSurface->setChecked(this->seifertSurface->isVisible());  
  
  QMenu *optionsMenu = menuBar()->addMenu("&Options");
  optionsMenu->addAction("&Thickness…", this, SLOT(setThickness()));
  optionsMenu->addSeparator();
  optionsMenu->addAction("Back&ground color…", this, SLOT(setBgColor()));
  optionsMenu->addAction("&Knot color…", this, SLOT(setKnotColor()));
  optionsMenu->addAction("Seifert surface &front color…", this, SLOT(setSeifertFrontColor()));
  optionsMenu->addAction("Seifert surface &back color…", this, SLOT(setSeifertBackColor()));

  addToolBarSeparator();
	this->registerAction(
		addToolbarAction("start.xpm", "Start smoothing", [this] { this->smooth(); }),
		[this](QAction &action) { action.setVisible(!this->smoothing); }
	);
	this->registerAction(
		addToolbarAction("stop.xpm", "Interrupt smoothing", [this] { this->stop(); }),
		[this](QAction &action) { action.setVisible(this->smoothing); }
	);
  addToolBarSeparator();
  addToolBarButton("math.xpm", "Show parameters", SLOT(math()));
  addToolBarSeparator();
  addToolBarButton("plus.xpm", "Shift Seifert surface along gradient", SLOT(bp_plus()));
  addToolBarButton("minus.xpm", "Shift Seifert surface along gradient", SLOT(bp_minus()));

	this->updateActions();
}

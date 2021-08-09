#include <QtWidgets/QAction>
#include <QtWidgets/QStatusBar>

#include "knotWindow_math.h"
#include "../../math/knot/knot_surface.h"
#include "../../math/seifert/seifert_surface.h"

void knotWindow::smooth() {
  if (!smoothing) {
    this->startSmooth(0, 4);
	}
}

void knotWindow::stop() {
  if (smoothing) {
    killTimer(timerId_smooth);
    smoothing = false;
    math_decreaseEnergy->setEnabled(true);
    math_stop->setEnabled(false);
    statusBar()->showMessage("Smoothing complete", 3000);
		this->updateActions();
  }
}

void knotWindow::doSmooth() {
  if (!continuousSmoothing) {
    if (redrawAfter > smoothSteps)
      redrawAfter = smoothSteps;
    smoothSteps -= redrawAfter;
    if (!smoothSteps)
      stop();
  }

  isSaved = false;
	this->knot.smooth(redrawAfter);
  this->knotSurface->destroy(false);
  this->seifertSurface->destroy(false);
  repaint3d();
  if (mth)
    mth->recompute();
}

void knotWindow::startSmooth(int st, int ra, bool cont) {
  smoothSteps = st;
  redrawAfter = ra;
  continuousSmoothing = cont;
  smoothing = true;
  math_decreaseEnergy->setEnabled(false);
  math_stop->setEnabled(true);

  timerId_smooth = startTimer(1);
  statusBar()->showMessage("Smoothing…");
	this->updateActions();
}

#include <QtWidgets/QAction>
#include <QtWidgets/QStatusBar>

#include "knotWindow_math.h"
#include "../../math/knotSurface/KnotSurface.h"
#include "../../math/seifert/SeifertSurface.h"

void knotWindow::smooth() {
  if (!this->smoothingThread.isRunning()) {
    this->startSmooth(0, 20);
	}
}

void knotWindow::stop() {
  if (this->smoothingThread.isRunning()) {
		this->smoothingThread.requestInterruption();
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

	for (int i = 0; i < redrawAfter; ++i) {
		this->knot.decreaseEnergy();
	}
}

void knotWindow::onKnotChanged() {
  if (this->knotSurface->destroy(false) || this->seifertSurface->destroy(false)) {
		this->repaint3d();
		if (mth) {
			mth->recompute();
		}
	}
}

void knotWindow::startSmooth(int st, int ra, bool cont) {
  smoothSteps = st;
  redrawAfter = ra;
  continuousSmoothing = cont;

	this->smoothingThread.start();
  statusBar()->showMessage("Smoothing…");
	this->updateActions();
}

SmoothingThread::SmoothingThread(knotWindow &knot) : knot(knot) {
	connect(this, &SmoothingThread::knotChanged, &knot, &knotWindow::onKnotChanged);
	connect(this, &SmoothingThread::finished, &knot, &knotWindow::updateActions);
}

void SmoothingThread::run() {
	this->setPriority(LowPriority);
	while (true) {
		if (this->isInterruptionRequested()) {
			this->quit();
			break;
		}
		this->msleep(20);
		knot.doSmooth();
		emit knotChanged();
	}
}

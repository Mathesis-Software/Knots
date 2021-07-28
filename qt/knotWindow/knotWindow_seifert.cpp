#include "knotWindow.h"

void knotWindow::bp_plus() {
	this->seifertStartPoint.move(
		this->knot->seifertGradient(this->seifertStartPoint), 0.02
	);
	this->seifertSurface->destroy();
	this->repaint3d();
}

void knotWindow::bp_minus() {
	this->seifertStartPoint.move(
		this->knot->seifertGradient(this->seifertStartPoint), -0.02
	);
	this->seifertSurface->destroy();
	this->repaint3d();
}

#include "knotWindow.h"
#include "../../math/seifert/seifert_surface.h"

void knotWindow::bp_plus() {
	this->seifertStartPoint.move(
		this->knot.knot().seifertGradient(this->seifertStartPoint), 0.02
	);
	this->seifertSurface->destroy(true);
	this->repaint3d();
}

void knotWindow::bp_minus() {
	this->seifertStartPoint.move(
		this->knot.knot().seifertGradient(this->seifertStartPoint), -0.02
	);
	this->seifertSurface->destroy(true);
	this->repaint3d();
}

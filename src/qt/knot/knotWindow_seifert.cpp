#include "knotWindow.h"
#include "../../math/seifert/SeifertSurface.h"

void knotWindow::bp_plus() {
	this->seifertStartPoint.move(
		KE::GL::SeifertSurface::gradient(this->seifertStartPoint, this->knot.snapshot()), 0.02
	);
	this->seifertSurface->destroy(true);
	this->repaint3d();
}

void knotWindow::bp_minus() {
	this->seifertStartPoint.move(
		KE::GL::SeifertSurface::gradient(this->seifertStartPoint, this->knot.snapshot()), -0.02
	);
	this->seifertSurface->destroy(true);
	this->repaint3d();
}

#include "knotWindow.h"

void knotWindow::bp_plus() {
  double shift[3];
  this->knot->getGradient(bp, shift);
  bp[0] += shift[0] / 50.0;
  bp[1] += shift[1] / 50.0;
  bp[2] += shift[2] / 50.0;
  this->seifertSurface->destroy();
  this->repaint3d();
}

void knotWindow::bp_minus() {
  double shift[3];
  this->knot->getGradient(bp, shift);
  bp[0] -= shift[0] / 50.0;
  bp[1] -= shift[1] / 50.0;
  bp[2] -= shift[2] / 50.0;
  this->seifertSurface->destroy();
  this->repaint3d();
}

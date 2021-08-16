#include "knotWindow.h"
#include "../setValue/setValue.h"
#include "../../math/knotSurface/KnotSurface.h"

void knotWindow::setThickness() {
  const double thickness = setDouble("Thickness", this->knotSurface->thickness(), 0.1, 10.0);
  if (thickness != this->knotSurface->thickness()) {
		this->knot.knotThickness = std::make_shared<double>(thickness);
    this->knotSurface->destroy(true);
		this->centralWidget()->update();
  }
}

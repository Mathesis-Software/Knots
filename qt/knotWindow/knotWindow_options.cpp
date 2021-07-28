#include "knotWindow.h"
#include "../../math/knot/knot_surface.h"
#include "../setValue/setValue.h"

void knotWindow::setThickness() {
  double old = thickness;
  thickness = setDouble("Thickness", thickness, 0.1, 10.0);
  if (old != thickness) {
    kSurf->destroy();
    ((KE::ThreeD::knot_surface*)kSurf) -> setThickness (0.05 * thickness);
    repaint3d();
  }
}

#include "knotWindow.h"

void knotWindow::readIt(std::istream &is) {
  is >> (Knot*)this;
  setWindowTitle (caption.c_str());
}

void knotWindow::saveIt(std::ostream &os) {
  os << "#KNOT " << windowTitle ().toStdString() << "\n#LENGTH " << this->points.size() << "\n";

	for (const auto &pt : this->points) {
    os << currMatr(0, 0) * pt.x +
          currMatr(1, 0) * pt.y +
          currMatr(2, 0) * pt.z << ' ' <<
          currMatr(0, 1) * pt.x +
          currMatr(1, 1) * pt.y +
          currMatr(2, 1) * pt.z << ' ' <<
          currMatr(0, 2) * pt.x +
          currMatr(1, 2) * pt.y +
          currMatr(2, 2) * pt.z << '\n';
	}
}

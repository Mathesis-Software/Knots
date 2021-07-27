#include "knotWindow.h"

void knotWindow::readIt (std::istream &is)
{
  is >> (knot*)this;
  setWindowTitle (caption.c_str());
}

void knotWindow::saveIt (std::ostream &os)
{
  os << "#KNOT " << windowTitle ().toStdString() << "\n#LENGTH " << length << "\n";

  for (std::size_t i = 0; i < length; i++)
    os << currMatr (0, 0) * points[i].x +
          currMatr (1, 0) * points[i].y +
          currMatr (2, 0) * points[i].z << ' ' <<
          currMatr (0, 1) * points[i].x +
          currMatr (1, 1) * points[i].y +
          currMatr (2, 1) * points[i].z << ' ' <<
          currMatr (0, 2) * points[i].x +
          currMatr (1, 2) * points[i].y +
          currMatr (2, 2) * points[i].z << '\n';
}

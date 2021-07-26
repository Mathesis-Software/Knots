#include "knotWindow.h"

void knotWindow::readIt (std::istream &is)
{
  is >> (knot*)this;
  setWindowTitle (Caption);
}

void knotWindow::saveIt (std::ostream &os)
{
  os << "#KNOT " << windowTitle ().toStdString() << "\n#LENGTH " << length << "\n";

  for (int i = 0; i < length; i++)
    os << currMatr (0, 0) * points [i] [0] +
          currMatr (1, 0) * points [i] [1] +
          currMatr (2, 0) * points [i] [2] << ' ' <<
          currMatr (0, 1) * points [i] [0] +
          currMatr (1, 1) * points [i] [1] +
          currMatr (2, 1) * points [i] [2] << ' ' <<
          currMatr (0, 2) * points [i] [0] +
          currMatr (1, 2) * points [i] [1] +
          currMatr (2, 2) * points [i] [2] << '\n';
}

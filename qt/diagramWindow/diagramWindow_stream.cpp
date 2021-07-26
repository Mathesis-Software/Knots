#include "diagramWindow.h"

void diagramWindow::readIt (std::istream &is)
{
  is >> this;
  setWindowTitle (Caption);
  isClosed = true;
  actions[0] -> setChecked (false);
  actions_convert -> setEnabled (true);
  actions_simplify -> setEnabled (true);
  actions_clear -> setEnabled (true);
}

void diagramWindow::saveIt (std::ostream &os)
{
  strcpy (Caption, windowTitle ().toStdString().c_str());
  os << this;
}

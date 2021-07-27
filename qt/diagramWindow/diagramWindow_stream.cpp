#include "diagramWindow.h"

void diagramWindow::readIt(std::istream &is) {
  is >> this;
  setWindowTitle(this->caption.c_str());
  isClosed = true;
  actions[0]->setChecked(false);
  actions_convert->setEnabled(true);
  actions_simplify->setEnabled(true);
  actions_clear->setEnabled(true);
}

void diagramWindow::saveIt(std::ostream &os) {
  this->caption = windowTitle().toStdString();
  os << this;
}

#include "diagramWindow.h"

void diagramWindow::simplify() {
  if (!isClosed)
    return;

  if (this->diagram.simplify(2)) {
    isSaved = false;
    centralWidget()->repaint();
  }
}

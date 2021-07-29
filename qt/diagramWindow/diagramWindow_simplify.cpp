#include "diagramWindow.h"

void diagramWindow::simplify() {
  if (!isClosed)
    return;

  if (Diagram::simplify(2)) {
    isSaved = false;
    centralWidget()->repaint();
  }
}

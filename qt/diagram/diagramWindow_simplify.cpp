#include "diagramWindow.h"

void diagramWindow::simplify() {
	if (this->diagramWidget()->diagram.simplify(2)) {
		this->isSaved = false;
		this->centralWidget()->repaint();
	}
}

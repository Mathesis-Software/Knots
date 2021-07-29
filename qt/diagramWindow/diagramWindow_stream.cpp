#include "diagramWindow.h"

void diagramWindow::saveIt(std::ostream &os) {
	this->diagram->save(os);
}

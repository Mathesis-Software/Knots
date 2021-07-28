#include "knotWindow.h"

void knotWindow::saveIt(std::ostream &os) {
	double matrix[3][3];
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			matrix[i][j] = this->currMatr(i, j);
		}
	}
	this->knot->save(os, matrix);
}

#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>

#include "../gl/GLWidget.h"
#include "knotWindow.h"

void knotWindow::saveIt(std::ostream &os) {
	double matrix[3][3];
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			matrix[i][j] = this->glWidget()->currMatr(i, j);
		}
	}
	const rapidjson::Document doc = this->knot.serialize(matrix);
	rapidjson::OStreamWrapper wrapper(os);
	rapidjson::Writer<rapidjson::OStreamWrapper> writer(wrapper);
	writer.SetMaxDecimalPlaces(5);
	doc.Accept(writer);
	this->_isSaved = true;
}

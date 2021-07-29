#include <cstring>

#include "knot.h"

namespace KE { namespace ThreeD {

Knot::Knot(std::istream &is) {
	char tmp[256];

	is.get(tmp, 32, ' ');
	if (strcmp(tmp, "#KNOT") != 0) {
		return;
	}

	is.get(tmp [0]);
	is.get(tmp, 255);
	this->caption = tmp;

	is.get(tmp, 32, ' ');
	int length;
	is >> length;
	if (strcmp(tmp, "\n#LENGTH") != 0 || length <= 0) {
		return;
	}

	this->points.clear();

	for (int i = 0; i < length; ++i) {
		double x, y, z;
		is >> x >> y >> z;
		this->points.push_back(Point(x, y, z));
		if (is.good()) {
			continue;
		}
		if (is.eof() && i == length - 1) {
			break;
		}
		this->points.clear();
		break;
	}

	if (!this->points.empty()) {
		this->center();
	}
	this->create_depend();
}

void Knot::save(std::ostream &os, const double matrix[3][3]) const {
	os << "#KNOT " << this->caption << "\n#LENGTH " << this->points.size() << "\n";
	for (const auto &pt : this->points) {
    os << matrix[0][0] * pt.x +
          matrix[1][0] * pt.y +
          matrix[2][0] * pt.z << ' ' <<
          matrix[0][1] * pt.x +
          matrix[1][1] * pt.y +
          matrix[2][1] * pt.z << ' ' <<
          matrix[0][2] * pt.x +
          matrix[1][2] * pt.y +
          matrix[2][2] * pt.z << '\n';
	}
}

}}

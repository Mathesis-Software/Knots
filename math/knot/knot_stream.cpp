#include <cstring>

#include "knot.h"

namespace KE { namespace ThreeD {

std::istream & operator >> (std::istream &is, Knot *K) {
	char tmp[256];

	is.get (tmp, 32, ' ');
	if (strcmp(tmp, "#KNOT") != 0) {
		return is;
	}

	is.get (tmp [0]);
	is.get (tmp, 255);
	K->caption = tmp;

	is.get (tmp, 32, ' ');
	int length;
	is >> length;
	if (strcmp(tmp, "\n#LENGTH") != 0 || length <= 0) {
		return is;
	}

	K->points.clear();

	for (int i = 0; i < length; ++i) {
		double x, y, z;
		is >> x >> y >> z;
		K->points.push_back(Point(x, y, z));
		if (is.good()) {
			continue;
		}
		if (is.eof() && i == length - 1) {
			break;
		}
		K->points.clear();
		break;
	}

	if (!K->points.empty()) {
		K->center();
	}

	return is;
}

std::ostream & operator << (std::ostream &os, Knot *K) {
	os << "#KNOT " << K->caption << "\n#LENGTH " << K->points.size() << "\n";
	for (const auto &pt : K->points) {
		os << pt.x << ' ' << pt.y << ' ' << pt.z << '\n';
	}
	return os;
}

}}

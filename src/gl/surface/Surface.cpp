#include <iomanip>
#include <sstream>

#include "Surface.h"

namespace KE { namespace GL {

Surface::Surface(bool stripped, bool showBackSide) : stripped(stripped), showBackSide(showBackSide) {}

Surface::~Surface() {
}

bool Surface::destroy(bool force) {
	if (force) {
		this->points.clear();
	}
	return force;
}

void Surface::addpoint(const ThreeD::Point &vertex, const ThreeD::Vector &normal) {
	this->points.push_back(SurfacePoint(vertex.x, vertex.y, vertex.z, normal.x, normal.y, normal.z));
}

std::string Color::stringValue() const {
	std::stringstream os;
	os
		<< std::hex << std::setfill('0') << std::setw(2) << (int)round(this->rgb[0] * 255)
		<< std::hex << std::setfill('0') << std::setw(2) << (int)round(this->rgb[1] * 255)
		<< std::hex << std::setfill('0') << std::setw(2) << (int)round(this->rgb[2] * 255);
	return os.str();
}

const Color Color::white(255, 255, 255);

std::shared_ptr<Color> Color::parse(const std::string &stringValue) {
	if (stringValue.size() != 6) {
		return nullptr;
	}
	unsigned int value;
	std::stringstream stream;
	stream << std::hex << stringValue;
	stream >> value;
	return std::make_shared<Color>((value >> 16) & 0xff, (value >> 8) & 0xff, value & 0xff);
}

}}

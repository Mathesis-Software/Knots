#include "Surface.h"

namespace KE { namespace GL {

Surface::Surface(bool stripped, bool showBackSide) : stripped(stripped), showBackSide(showBackSide) {
	visible = 0;
}

Surface::~Surface() {
}

void Surface::destroy() {
	this->points.clear();
}

void Surface::addpoint(const ThreeD::Point &vertex, const ThreeD::Vector &normal) {
	this->points.push_back(SurfacePoint(vertex.x, vertex.y, vertex.z, normal.x, normal.y, normal.z));
}

}}

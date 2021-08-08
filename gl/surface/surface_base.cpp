#include "surface.h"

namespace KE { namespace GL {

Surface::Surface(bool stripped, bool showBackSide) : stripped(stripped), showBackSide(showBackSide) {
  frontRGB[0] = 1.0;
  frontRGB[1] = 1.0;
  frontRGB[2] = 1.0;
  backRGB[0] = 1.0;
  backRGB[1] = 1.0;
  backRGB[2] = 1.0;

  visible = 0;
}

Surface::~Surface() {
}

void Surface::destroy() {
	this->points.clear();
}

void Surface::setFrontRGB(const float rgb[3]) {
  frontRGB[0] = rgb[0];
  frontRGB[1] = rgb[1];
  frontRGB[2] = rgb[2];
}

void Surface::setBackRGB(const float rgb[3]) {
  backRGB[0] = rgb[0];
  backRGB[1] = rgb[1];
  backRGB[2] = rgb[2];
}

void Surface::addpoint(const ThreeD::Point &vertex, const ThreeD::Vector &normal) {
  this->points.push_back(SurfacePoint(vertex.x, vertex.y, vertex.z, normal.x, normal.y, normal.z));
}

}}

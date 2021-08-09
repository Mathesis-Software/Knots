#ifndef __SURFACE_H__
#define __SURFACE_H__

#include <vector>

#include "../../math/knot/primitives.h"

namespace KE { namespace GL {

class Surface {

private:
  struct SurfacePoint {
    float vertex[3];
    float normal[3];

		SurfacePoint(float v0, float v1, float v2, float n0, float n1, float n2) {
			vertex[0] = v0;
			vertex[1] = v1;
			vertex[2] = v2;
			normal[0] = n0;
			normal[1] = n1;
			normal[2] = n2;
		}
  };

  const bool stripped;
  const bool showBackSide;
  bool visible;

	std::vector<SurfacePoint> points;
  float frontRGB[3], backRGB[3];

protected:
  void destroy();
  virtual void calculate() = 0;
  void addpoint(const ThreeD::Point &vertex, const ThreeD::Vector &normal);
  
public:
  Surface(bool stripped, bool showBackSide);
  virtual ~Surface();

  bool isVisible() { return this->visible; }
  void show() { this->visible = true; }
  void hide() { this->visible = false; }

  void paint();

  const float *getFrontRGB() const { return this->frontRGB; }
  const float *getBackRGB() const { return this->backRGB; }
  void setFrontRGB(const float rgb[3]);
  void setBackRGB(const float rgb[3]);

private:
	Surface(const Surface&) = delete;
	Surface& operator = (const Surface&) = delete;
};

}}

#endif /* __SURFACE_H__ */

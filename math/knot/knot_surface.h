#ifndef __KNOT_SURFACE_H__
#define __KNOT_SURFACE_H__

#include "../../gl/surface/surface.h"

namespace KE {

namespace ThreeD {

class Knot;

}

namespace GL {

class KnotSurface : public Surface {

private:
	const ThreeD::Knot &knot;
  double thickness;

public:
  KnotSurface(const ThreeD::Knot &knot, double thickness);
  void calculate(void);
  void setThickness(double t) {thickness = t;}

private:
	KnotSurface(const KnotSurface&) = delete;
	KnotSurface& operator = (const KnotSurface&) = delete;
};

}}

#endif /* __KNOT_SURFACE_H__ */

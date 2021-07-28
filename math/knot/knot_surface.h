#ifndef __KNOT_SURFACE_H__
#define __KNOT_SURFACE_H__

#include "../../gl/surface/surface.h"

namespace KE { namespace ThreeD {

class Knot;

class knot_surface : public surface {

private:
	KE::ThreeD::Knot *Parent;
  double thickness;

public:
  knot_surface(KE::ThreeD::Knot*, double);
  void calculate(void);
  void setThickness(double t) {thickness = t;}
};

}}

#endif /* __KNOT_SURFACE_H__ */

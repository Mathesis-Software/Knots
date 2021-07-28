#ifndef __KNOT_SURFACE_H__
#define __KNOT_SURFACE_H__

#include "../../gl/surface/surface.h"

class Knot;

class knot_surface : public surface {

private:
  Knot *Parent;
  double thickness;

public:
  knot_surface (Knot*, double);
  void calculate (void);
  void setThickness (double t)
    {thickness = t;};
};

#endif /* __KNOT_SURFACE_H__ */

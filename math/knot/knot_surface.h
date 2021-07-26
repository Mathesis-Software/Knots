#ifndef __KNOT_SURFACE_H__
#define __KNOT_SURFACE_H__

#include "../../gl/surface/surface.h"

class knot;

class knot_surface : public surface {

private:

  knot *Parent;
  double thickness;

public:

  knot_surface (knot*, double);
  void calculate (void);
  void setThickness (double t)
    {thickness = t;};
};

#endif /* __KNOT_SURFACE_H__ */

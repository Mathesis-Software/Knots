#ifndef __SEIFERT_SURFACE_H__
#define __SEIFERT_SURFACE_H__

#include "../knot/primitives.h"
#include "../../gl/surface/surface.h"

class seifert;

namespace KE {

namespace ThreeD {

class Knot;

}

namespace GL {

class SeifertSurface : public Surface {
  private:
    const ThreeD::Knot &base;
    const ThreeD::Point &startPoint;

    void addTriangles(seifert *s);
    void calculate();

  public:
    SeifertSurface(const ThreeD::Knot &base, const ThreeD::Point &startPoint);
};

}}

#endif /* __SEIFERT_SURFACE_H__ */

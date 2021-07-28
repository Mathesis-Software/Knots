#ifndef __SEIFERT_SURFACE_H__
#define __SEIFERT_SURFACE_H__

#include "../../gl/surface/surface.h"

class seifert;
class seifert_base;

namespace KE { namespace GL {

class SeifertSurface : public Surface {
  private:
    const seifert_base &base;
    double *start_point;

    void addTriangles(seifert *s);
    void calculate();

  public:
    SeifertSurface(const seifert_base &base, double*);
};

}}

#endif /* __SEIFERT_SURFACE_H__ */

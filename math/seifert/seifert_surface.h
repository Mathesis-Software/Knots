#ifndef __SEIFERT_SURFACE_H__
#define __SEIFERT_SURFACE_H__

#include "../../gl/surface/surface.h"

class seifert;
class seifert_base;

class seifert_surface : public surface {

  private:

    seifert_base *base;
    double *start_point;

    void addTriangles (seifert *s);
    void calculate (void);

  public:

    seifert_surface (seifert_base*, double*);
};

#endif /* __SEIFERT_SURFACE_H__ */

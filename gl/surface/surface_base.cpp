#include "surface.h"

surface::surface (void)
{
  frontRGB [0] = 1.0;
  frontRGB [1] = 1.0;
  frontRGB [2] = 1.0;
  backRGB [0] = 1.0;
  backRGB [1] = 1.0;
  backRGB [2] = 1.0;

  visible = 0;

  points = new std::list<surfpoint*>;
}

surface::~surface (void)
{
  destroy ();
  delete points;
}

void surface::destroy (void)
{
  for (std::list<surfpoint*>::iterator it = points -> begin ();
       it != points -> end ();
       it++)
    delete (*it);

  points -> clear ();
}

void surface::setFrontRGB (const float *rgb)
{
  frontRGB [0] = rgb [0];
  frontRGB [1] = rgb [1];
  frontRGB [2] = rgb [2];
}

void surface::setBackRGB (const float *rgb)
{
  backRGB [0] = rgb [0];
  backRGB [1] = rgb [1];
  backRGB [2] = rgb [2];
}

void surface::addpoint (const float v0, const float v1, const float v2,
                        const float n0, const float n1, const float n2)
{
  points -> push_back (new surfpoint);

  points -> back () -> vertex [0] = v0;
  points -> back () -> vertex [1] = v1;
  points -> back () -> vertex [2] = v2;
  points -> back () -> normal [0] = n0;
  points -> back () -> normal [1] = n1;
  points -> back () -> normal [2] = n2;
}

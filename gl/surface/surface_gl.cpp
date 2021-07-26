#include "surface.h"
#include <GL/gl.h>

void surface::paint (void)
{
  // Поверхность перерисовывается, если она видима.
  if (visible)
  {
    // Если нужно, сначала делаем вычисления.
    if (points -> empty())
      calculate ();

    // Устанавливаем цвета обеих сторон поверхности.
    glMaterialfv (GL_FRONT, GL_DIFFUSE, frontRGB);
    glMaterialfv (GL_BACK, GL_DIFFUSE, backRGB);

    // Двусторонняя ли поверхность?
    glLightModeli (GL_LIGHT_MODEL_TWO_SIDE, (sides == Both));

    // Связаны ли треугольники?
    glBegin (stripped ? GL_TRIANGLE_STRIP : GL_TRIANGLES);

    for (std::list<surfpoint*>::iterator it = points -> begin ();
         it != points -> end ();
         it++)
    {
      glNormal3fv ((*it) -> normal);
      glVertex3fv ((*it) -> vertex);
    }
 
    glEnd ();
  }
}

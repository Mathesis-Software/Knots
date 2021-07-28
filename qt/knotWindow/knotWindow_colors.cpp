#include "knotWindow.h"

#include "../../math/knot/knot_surface.h"
#include "../../math/seifert/seifert_surface.h"

void knotWindow::setBgColor ()
{
  setup3dMainWidgetColor *setup = new setup3dMainWidgetColor(this, backgroundRGB);
  setup -> setWindowTitle ("Background color");
  setup -> exec ();
  delete setup;
}

void knotWindow::setKnotColor ()
{
  setup3dMainWidgetColor *setup =
    new setup3dMainWidgetColor(this, this->knotSurface->getFrontRGB(), this->knotSurface->isVisible());
  setup -> setWindowTitle ("Knot color");
  setup -> exec ();
  delete setup;
}

void knotWindow::setSeifertFrontColor ()
{
  setup3dMainWidgetColor *setup =
    new setup3dMainWidgetColor(this, this->seifertSurface->getFrontRGB(), this->seifertSurface->isVisible());
  setup -> setWindowTitle ("Seifert surface color");
  setup -> exec ();
  delete setup;
}

void knotWindow::setSeifertBackColor ()
{
  setup3dMainWidgetColor *setup =
    new setup3dMainWidgetColor(this, this->seifertSurface->getBackRGB(), this->seifertSurface->isVisible());
  setup -> setWindowTitle ("Seifert surface color");
  setup -> exec ();
  delete setup;
}

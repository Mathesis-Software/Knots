#include "knotWindow.h"

#include "../../math/knot/knot_surface.h"
#include "../../math/seifert/seifert_surface.h"

void knotWindow::setBgColor ()
{
  setupGLWidgetColor *setup = new setupGLWidgetColor(this, backgroundRGB);
  setup -> setWindowTitle ("Background color");
  setup -> exec ();
  delete setup;
}

void knotWindow::setKnotColor ()
{
  setupGLWidgetColor *setup =
    new setupGLWidgetColor(this, this->knotSurface->getFrontRGB(), this->knotSurface->isVisible());
  setup -> setWindowTitle ("Knot color");
  setup -> exec ();
  delete setup;
}

void knotWindow::setSeifertFrontColor ()
{
  setupGLWidgetColor *setup =
    new setupGLWidgetColor(this, this->seifertSurface->getFrontRGB(), this->seifertSurface->isVisible());
  setup -> setWindowTitle ("Seifert surface color");
  setup -> exec ();
  delete setup;
}

void knotWindow::setSeifertBackColor ()
{
  setupGLWidgetColor *setup =
    new setupGLWidgetColor(this, this->seifertSurface->getBackRGB(), this->seifertSurface->isVisible());
  setup -> setWindowTitle ("Seifert surface color");
  setup -> exec ();
  delete setup;
}

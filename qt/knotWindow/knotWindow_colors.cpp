#include "knotWindow.h"

void knotWindow::setBgColor ()
{
  setup3dMainWidgetColor *setup = new setup3dMainWidgetColor (this, backgroundRGB);
  setup -> setWindowTitle ("Background color");
  setup -> exec ();
  delete setup;
}

void knotWindow::setKnotColor ()
{
  setup3dMainWidgetColor *setup =
    new setup3dMainWidgetColor (this, kSurf -> getFrontRGB (), kSurf -> isVisible ());
  setup -> setWindowTitle ("Knot color");
  setup -> exec ();
  delete setup;
}

void knotWindow::setSeifertFrontColor ()
{
  setup3dMainWidgetColor *setup =
    new setup3dMainWidgetColor (this, sSurf -> getFrontRGB (), sSurf -> isVisible ());
  setup -> setWindowTitle ("Seifert surface color");
  setup -> exec ();
  delete setup;
}

void knotWindow::setSeifertBackColor ()
{
  setup3dMainWidgetColor *setup =
    new setup3dMainWidgetColor (this, sSurf -> getBackRGB (), sSurf -> isVisible ());
  setup -> setWindowTitle ("Seifert surface color");
  setup -> exec ();
  delete setup;
}

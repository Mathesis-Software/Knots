#include <QtWidgets/qmenubar.h>
#include <QtWidgets/qmessagebox.h>
#include <QtGui/qpixmap.h>
#include <QtCore/QTimerEvent>

#include "knotWindow.h"
#include "knotWindow_math.h"
#include "../../math/knot/knot_surface.h"

void knotWindow::init (void)
{
  bp [0] = 0.0;
  bp [1] = 0.0;
  bp [2] = 0.4;
  
  kSurf = new knot_surface (this, 0.05);
  addSurface (kSurf);
  sSurf = new seifert_surface (this, bp);
  addSurface (sSurf);

  float rgb [3];
  rgb [0] = 1.0;
  rgb [1] = 1.0;
  rgb [2] = 1.0;
  kSurf -> setFrontRGB (rgb);
  rgb [0] = 1.0;
  rgb [1] = 1.0;
  rgb [2] = 1.0;
  sSurf -> setFrontRGB (rgb);
  rgb [0] = 0.5;
  rgb [1] = 0.5;
  rgb [2] = 0.5;
  sSurf -> setBackRGB (rgb);

  kSurf -> show ();

  thickness = 1.0;
  smoothing = false;
  mth = NULL;

  initMenu ();
  complete ();

  setWindowIcon (QPixmap ((QString) getenv ("KNOTEDITOR_PIXMAPS") + "/trefoil.xpm"));
}

knotWindow::knotWindow (std::istream &is) :
  knot ()
{
  init ();
  readIt (is);
}

knotWindow::knotWindow (diagramWindow *d) :
  knot (d, d -> width (), d -> height ())
{
  init ();
  setWindowTitle(caption.c_str());
  isSaved = false;
}

knotWindow::~knotWindow (void)
{
  if (mth)
    delete mth;

  delete mathMenu;
  delete viewMenu;
}

void knotWindow::timerEvent (QTimerEvent *te)
{
  if ( smoothing && (te -> timerId () == timerId_smooth) )
    doSmooth ();
  else
    dddWindow::timerEvent (te);
}

void knotWindow::switchShowKnot() {
  if (kSurf -> isVisible ()) {
    kSurf -> hide ();
    view_showKnot -> setChecked (false);
  } else {
    kSurf -> show ();
    view_showKnot -> setChecked (true);
  }

  repaint3d ();
}

void knotWindow::switchShowSeifert ()
{
  if (sSurf -> isVisible ()) {
    sSurf -> hide ();
    view_showSeifertSurface->setChecked(false);
  } else {
    sSurf -> show ();
    view_showSeifertSurface->setChecked(true);
  }

  repaint3d ();
}

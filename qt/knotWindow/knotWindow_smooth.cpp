#include <QtWidgets/qstatusbar.h>

#include "knotWindow_math.h"

void knotWindow::smooth ()
{
  if (!smoothing)
    startSmooth (0, 4);
}

void knotWindow::stop (void)
{
  if (smoothing)
  {
    killTimer (timerId_smooth);
    smoothing = false;
    math_decreaseEnergy -> setEnabled (true);
    math_stop -> setEnabled (false);
    statusBar () -> showMessage ("Smoothing completed.", 3000);
  }
}

void knotWindow::doSmooth (void)
{
  if (!continuousSmoothing)
  {
    if (redrawAfter > smoothSteps)
      redrawAfter = smoothSteps;
    smoothSteps -= redrawAfter;
    if (!smoothSteps)
      stop ();
  }

  isSaved = false;
  for (int i = 0; i < redrawAfter; i++)
    Knot::decreaseEnergy ();
  center ();
  kSurf -> destroy ();
  sSurf -> destroy ();
  repaint3d ();
  if (mth)
    mth -> recompute ();
}

void knotWindow::startSmooth (int st, int ra, bool cont)
{
  smoothSteps = st;
  redrawAfter = ra;
  continuousSmoothing = cont;
  smoothing = true;
  math_decreaseEnergy -> setEnabled (false);
  math_stop -> setEnabled (true);

  timerId_smooth = startTimer (1);
  statusBar () -> showMessage ("Smoothing...");
}

#include "diagramWindow.h"

void diagramWindow::simplify (void)
{
  if (!isClosed)
    return;

  if (diagram::simplify (2))
  {
    isSaved = false;
    centralWidget () -> repaint ();
    //centralWidget () -> repaint (false);
  }
}

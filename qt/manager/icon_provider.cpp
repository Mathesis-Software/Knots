#include <stdlib.h>
#include <QtGui/qbitmap.h>
#include <QtGui/qpixmap.h>

#include "icon_provider.h"

static QPixmap *folderIcon = 0;
static QPixmap *diagramIcon = 0;
static QPixmap *knotIcon = 0;
static QPixmap *emptyIcon = 0;

keFileIconProvider::keFileIconProvider (void)
{
  folderIcon = new QPixmap
    ((QString) getenv ("KNOTEDITOR_PIXMAPS") + "/file_folder.xpm");
  diagramIcon = new QPixmap
    ((QString) getenv ("KNOTEDITOR_PIXMAPS") + "/file_diagram.xpm");
  knotIcon = new QPixmap
    ((QString) getenv ("KNOTEDITOR_PIXMAPS") + "/file_knot.xpm");
  emptyIcon = new QPixmap (folderIcon -> width (), 1);
  QBitmap mask (emptyIcon -> width (), 1);
  mask.fill (Qt::color0);
  emptyIcon -> setMask (mask);
}

const QPixmap *keFileIconProvider::pixmap (const QFileInfo &finfo)
{
  if (finfo.isDir ())
    return folderIcon;

  if (finfo.suffix () == "knt")
    return knotIcon;
  if (finfo.suffix () == "dgr")
    return diagramIcon;

  return emptyIcon;
}

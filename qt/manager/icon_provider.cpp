#include "icon_provider.h"

keFileIconProvider *keFileIconProvider::_instance = 0;

keFileIconProvider *keFileIconProvider::instance() {
  if (_instance == 0) {
    _instance = new keFileIconProvider();
  }
  return _instance;
}

keFileIconProvider::keFileIconProvider() :
  diagramIcon((QString)getenv("KNOTEDITOR_PIXMAPS") + "/file_diagram.xpm"),
  knotIcon((QString)getenv("KNOTEDITOR_PIXMAPS") + "/file_knot.xpm"),
  emptyIcon() {
}

QIcon keFileIconProvider::icon(const QFileInfo &finfo) const {
  if (finfo.suffix() == "knt") {
    return knotIcon;
  } else if (finfo.suffix() == "dgr") {
    return diagramIcon;
  }

  return emptyIcon;
}

#include "iconProvider.h"

namespace KE { namespace Qt {

FileIconProvider *FileIconProvider::_instance = 0;

FileIconProvider *FileIconProvider::instance() {
	if (_instance == 0) {
		_instance = new FileIconProvider();
	}
	return _instance;
}

FileIconProvider::FileIconProvider() :
	diagramIcon(":images/diagram.svg"),
	knotIcon(":images/trefoil.png") {
}

QIcon FileIconProvider::icon(const QFileInfo &finfo) const {
	if (finfo.suffix() == "knt") {
		return knotIcon;
	} else if (finfo.suffix() == "dgr") {
		return diagramIcon;
	}

	return QFileIconProvider::icon(finfo);
}

}}

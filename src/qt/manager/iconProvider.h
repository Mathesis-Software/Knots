#ifndef __ICON_PROVIDER_H__
#define __ICON_PROVIDER_H__

#include <QtWidgets/QFileIconProvider>

namespace KE { namespace Qt {

class FileIconProvider : public QFileIconProvider {

public:
  static FileIconProvider *instance();

private:
	static FileIconProvider *_instance;

private:
  FileIconProvider();

public:
  QIcon icon(const QFileInfo&) const override;

private:
  const QIcon diagramIcon;
  const QIcon knotIcon;
};

}}

#endif /* __ICON_PROVIDER_H__ */

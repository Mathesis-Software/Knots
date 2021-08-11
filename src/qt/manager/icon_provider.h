#ifndef __ICON_PROVIDER_H__
#define __ICON_PROVIDER_H__

#include <QtWidgets/QFileIconProvider>

class keFileIconProvider : public QFileIconProvider {

public:
  static keFileIconProvider *instance();

private:
	static keFileIconProvider *_instance;

private:
  keFileIconProvider();

public:
  QIcon icon(const QFileInfo&) const override;

private:
  const QIcon diagramIcon;
  const QIcon knotIcon;
  const QIcon emptyIcon;
};

#endif /* __ICON_PROVIDER_H__ */

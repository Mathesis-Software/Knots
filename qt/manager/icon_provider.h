#include <QtWidgets/QFileIconProvider>

class keFileIconProvider : public QFileIconProvider
{

public:

  keFileIconProvider (void);
  const QPixmap *pixmap (const QFileInfo&);
};

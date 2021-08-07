#ifndef __MANAGER_H__
#define __MANAGER_H__

#include <QtWidgets/QMenuBar>

class keManager : public QMenuBar {

private:
  QMenu *fileMenu, *aboutMenu;

private:
  void closeEvent(QCloseEvent*);

  void new_diagram();
  void open();
  void close_all_windows();
  void exit();

  void about();
  void about_qt();

public:
  keManager();
  ~keManager();
};

#endif /* __MANAGER_H__ */

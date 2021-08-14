#ifndef __MANAGER_H__
#define __MANAGER_H__

#include <memory>

#include <QtWidgets/QMainWindow>

namespace KE { namespace Qt {

class ManagerWindow : public QMainWindow {

private:
	QMenu *fileMenu;

private:
  void closeEvent(QCloseEvent*);

  void new_diagram();
  void open();
  void close_all_windows();
  void exit();

  void about();

public:
  ManagerWindow();
  ~ManagerWindow();
};

}}

#endif /* __MANAGER_H__ */

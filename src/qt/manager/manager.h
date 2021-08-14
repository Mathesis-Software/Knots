#ifndef __MANAGER_H__
#define __MANAGER_H__

#include <memory>

#include <QtWidgets/QMainWindow>

namespace KE { namespace Qt {

class ManagerWindow : public QMainWindow {

public:
  static void newDiagram();
  static bool openFile();

private:
	QMenu *fileMenu;

private:
  void exit();

public:
  ManagerWindow();
  ~ManagerWindow();
};

}}

#endif /* __MANAGER_H__ */

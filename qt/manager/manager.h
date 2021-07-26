#ifndef __MANAGER_H__
#define __MANAGER_H__

#include <QtWidgets/QApplication>
#include <QtWidgets/QMenuBar>

class keManager : public QMenuBar {

  Q_OBJECT

private:

  QMenu *fileMenu, *aboutMenu;

  void closeEvent (QCloseEvent*);

private slots:

  void new_diagram ();
  void open ();
  void close_all_windows ();
  void exit ();

  void about ();
  void about_qt ();

public:

  keManager (void);
  ~keManager (void);
};

#endif /* __MANAGER_H__ */

#ifndef __ABSTRACTWINDOW_H__
#define __ABSTRACTWINDOW_H__

#include <fstream>
#include <list>

#include <QtPrintSupport/QPrinter>
#include <QtWidgets/qmainwindow.h>
#include <QtWidgets/qtoolbutton.h>

class abstractWindow : public QMainWindow {

  Q_OBJECT

private:
  QToolBar *toolbar;

protected:

  bool isSaved;

  void closeEvent (QCloseEvent*);

  virtual void printIt (QPrinter*) = 0;
  virtual void saveIt (std::ostream&) = 0;

  int askForSave (void);

  QToolButton *addToolBarButton (const char*, const char*,
		                 const char*, const char* = 0);
  void addToolBarSeparator (void);
  void complete (bool = false);

private slots:

  void save_as ();
  void print ();
  void rename ();

public slots:

  void close ();

public:

  abstractWindow (void);
  virtual ~abstractWindow (void);

  virtual bool isEmpty (void) = 0;

  virtual const char *mask (void) = 0;

  static std::list<abstractWindow*> AWRegister;
  static bool removeAll (void);
};

#endif /* __ABSTRACTWINDOW_H__ */

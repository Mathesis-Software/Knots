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
	std::map<QAction*,std::function<void(QAction&)>> actionsMap;

protected:
  bool isSaved;

  void closeEvent(QCloseEvent*);

  virtual void printIt(QPrinter*) = 0;
  virtual void saveIt(std::ostream&) = 0;

  int askForSave(void);

  QAction *addToolbarAction(const QString &iconFilename, const QString &text, const std::function<void()> &functor);
  QToolButton *addToolBarButton(const QString&, const char*, const char*);
  void addToolBarSeparator(void);
  void complete();

	void registerAction(QAction *action, std::function<void(QAction&)> controller);

private slots:
  void save_as();
  void print();
  void rename();

public slots:
  void close();

public:
  abstractWindow();
  virtual ~abstractWindow();

	void updateActions();

  virtual bool isEmpty() const = 0;

  virtual const char *mask() const = 0;

  static std::list<abstractWindow*> AWRegister;
  static bool removeAll();
};

#endif /* __ABSTRACTWINDOW_H__ */

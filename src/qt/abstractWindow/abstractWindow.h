#ifndef __ABSTRACTWINDOW_H__
#define __ABSTRACTWINDOW_H__

#include <fstream>
#include <list>

#include <QtPrintSupport/QPrinter>
#include <QtWidgets/QMainWindow>

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
  void addToolbarSeparator();
  void complete();

	void registerAction(QAction *action, std::function<void(QAction&)> controller);

private slots:
  void save_as();
  void print();

public:
  abstractWindow();
  virtual ~abstractWindow();

	virtual void updateActions();

  virtual bool isEmpty() const = 0;

  virtual const char *mask() const = 0;

  static std::list<abstractWindow*> AWRegister;
  static bool closeAllWindows();

protected:
  virtual void rename() = 0;
};

#endif /* __ABSTRACTWINDOW_H__ */

#ifndef __KNOTWINDOW_MATH_H__
#define __KNOTWINDOW_MATH_H__

#include <QtWidgets/qcheckbox.h>
#include <QtWidgets/qlabel.h>
//#include <qwindow.h>
#include <QtWidgets/qdialog.h>

#include "knotWindow.h"

class paramWindow : public QDialog {
  
  Q_OBJECT

private:
  
  knotWindow *Parent;
  int nLabels;
  class parameterLabel **pLabels;
  void closeEvent (QCloseEvent*);

public:

  paramWindow (knotWindow*);
  ~paramWindow (void);
  void recompute (void);
};

class parameterLabel : public QWidget {

Q_OBJECT

private:

  QLabel *lbl;
  QCheckBox *chkbox;
  parameter *prm;

private slots:

  void doit ();

public:

  parameterLabel (QDialog*, parameter*, int, int);
  ~parameterLabel (void);

  void renew (void);
};

#endif /* __KNOTWINDOW_MATH_H__ */

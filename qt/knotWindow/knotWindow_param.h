#ifndef __KNOTWINDOW_PARAM_H__
#define __KNOTWINDOW_PARAM_H__

#include <QtWidgets/qdialog.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qpushbutton.h>

class setupSmooth : public QDialog {

Q_OBJECT
  
private:

  setupSmooth (int*, int*);
  ~setupSmooth (void);

  QLineEdit *steps_label, *redraw_label;
  QPushButton *startButton, *cancelButton;
  
  int *steps, *redraw;

private slots:

  void test ();

public:

  static bool set (int*, int*);
};

#endif /* __KNOTWINDOW_PARAM_H__ */

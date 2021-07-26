#ifndef __SETUPCOLOR_H__
#define __SETUPCOLOR_H__

#include <cstdbool>

#include <QtWidgets/qdialog.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qslider.h>

class setupColor : public QDialog {

Q_OBJECT
  
public:

  setupColor (float*, bool = true);
  ~setupColor (void);

private:
  
  QLabel *rText, *gText, *bText;
  QLabel *rValue, *gValue, *bValue;
  QSlider *rSlider, *gSlider, *bSlider;
  QPushButton *okButton, *cancelButton;
  
  bool applyJust;
  float *RGB, oldRGB [3];

private slots:

  void rChanged (int);
  void gChanged (int);
  void bChanged (int);
  void restore ();
  virtual void apply () = 0;
};

#endif /* __SETUPCOLOR_H__ */

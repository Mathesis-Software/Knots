#include "../../math/headers/parameter.h"
#include "knotWindow_math.h"

void knotWindow::math() {
  if (mth) {
    mth -> show();
    mth -> raise();
    return;
  }

  mth = new paramWindow (this);
  mth -> setWindowTitle ((QString) "Parameters for " + windowTitle ());
  mth -> show ();
}

paramWindow::paramWindow (knotWindow *p) {
  Parent = p;
  nLabels = Parent -> parameterList.size();
  pLabels = new parameterLabel* [nLabels];
 
  int index = 0;
  for (auto param : Parent->parameterList) {
    pLabels[index] = new parameterLabel (this, param, 20, 15 + 30 * index);
		index += 1;
  }

  setFixedSize (380, 25 + 30 * nLabels);
}

void paramWindow::recompute (void) {
  for (int i = 0; i < nLabels; i++) {
    pLabels [i] -> renew ();
  }
}

paramWindow::~paramWindow (void)
{
  for (int i = 0; i < nLabels; i++)
    delete pLabels [i];

  delete[] pLabels;
}

void paramWindow::closeEvent (QCloseEvent*)
{
  Parent -> mth = NULL;
  delete this;
}

parameterLabel::parameterLabel(QDialog* parent, std::shared_ptr<parameter> param, int x, int y) : QWidget(parent), prm(param) {
  lbl = new QLabel (this);
  lbl -> setGeometry (240, 0, 100, 25);
  lbl -> setFrameStyle (QFrame::Panel | QFrame::Sunken);
  //lbl -> setBackgroundColor (Qt::lightGray);

  chkbox = new QCheckBox (param->name.c_str(), this);
  chkbox -> setGeometry (0, 0, 220, 25);
  connect (chkbox, SIGNAL (clicked ()), SLOT (doit ()));

  setGeometry (x, y, 340, 25);
}

parameterLabel::~parameterLabel (void)
{
  delete lbl;
  delete chkbox;
}

void parameterLabel::doit (void)
{
  if ( chkbox -> isChecked () ) {
    //lbl -> setBackgroundColor (Qt::white);
    QString s;
    s.sprintf ("%f", prm -> value ());
    lbl -> setText (s);
  } else {
    //lbl -> setBackgroundColor (Qt::lightGray);
    lbl -> setText (0);
  }
}

void parameterLabel::renew (void)
{
  if ( chkbox -> isChecked () )
  {
    QString s;
    s.sprintf ("%f", prm -> value ());
    lbl -> setText (s);
  }
}

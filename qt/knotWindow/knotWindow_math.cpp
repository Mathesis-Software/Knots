#include "knotWindow_math.h"

void knotWindow::math() {
  if (mth) {
    mth->show();
    mth->raise();
    return;
  }

  mth = new paramWindow (this);
  mth->setWindowTitle ((QString) "Parameters for " + windowTitle ());
  mth->show ();
}

paramWindow::paramWindow (knotWindow *p) {
  Parent = p;
  nLabels = Parent->knot->computables.size();
  pLabels = new parameterLabel* [nLabels];
 
  int index = 0;
  for (auto computable : Parent->knot->computables) {
    pLabels[index] = new parameterLabel(this, computable, 20, 15 + 30 * index);
		index += 1;
  }

  setFixedSize (380, 25 + 30 * nLabels);
}

void paramWindow::recompute() {
  for (int i = 0; i < nLabels; i++) {
    pLabels [i]->renew ();
  }
}

paramWindow::~paramWindow() {
  for (int i = 0; i < nLabels; i++) {
    delete pLabels [i];
	}

  delete[] pLabels;
}

void paramWindow::closeEvent(QCloseEvent*) {
  Parent->mth = NULL;
  delete this;
}

parameterLabel::parameterLabel(QDialog* parent, std::shared_ptr<KE::ThreeD::Knot::Computable> computable, int x, int y) : QWidget(parent), computable(computable) {
  lbl = new QLabel (this);
  lbl->setGeometry (240, 0, 100, 25);
  lbl->setFrameStyle(QFrame::Panel | QFrame::Sunken);

  chkbox = new QCheckBox (computable->name.c_str(), this);
  chkbox->setGeometry (0, 0, 220, 25);
  connect (chkbox, SIGNAL (clicked ()), SLOT (doit ()));

  setGeometry (x, y, 340, 25);
}

parameterLabel::~parameterLabel() {
  delete lbl;
  delete chkbox;
}

void parameterLabel::doit() {
  if (chkbox->isChecked()) {
    lbl->setText(QString::number(computable->value()));
  } else {
    lbl->setText(QString());
  }
}

void parameterLabel::renew() {
  if (chkbox->isChecked()) {
    lbl->setText(QString::number(computable->value()));
  }
}

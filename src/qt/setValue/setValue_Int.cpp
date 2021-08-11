#include <stdio.h>

#include "setValue.h"

setupInt::setupInt (int* tmp, int min, int max) : QDialog (0) {
  this->setModal(true);
  setFixedSize (200, 115);

  label = new QLineEdit (this);
  label -> setGeometry (50, 20, 100, 30);
  label -> setMaxLength (10);
  QString s;
  s.setNum (*tmp);
  label -> setText (s);

  okButton = new QPushButton ("OK", this);
  okButton -> setGeometry (20, 70, 70, 30);
  connect (okButton, SIGNAL (clicked()), SLOT (test()));
  connect (label, SIGNAL (returnPressed()), SLOT (test()));
  cancelButton = new QPushButton ("Cancel", this);
  cancelButton -> setGeometry (110, 70, 70, 30);
  connect (cancelButton, SIGNAL (clicked()), SLOT (reject()));

  current = tmp;
  minValue = min;
  maxValue = max;
}

setupInt::~setupInt (void)
{
  delete label;
  delete okButton;
  delete cancelButton;
}

void setupInt::test ()
{
  char trash [11];
  int *result = new int;
  if (sscanf (label -> text ().toStdString().c_str(), "%d%s", result, trash) == 1)
  {
    if ( (*result >= minValue) && (*result <= maxValue) )
    {
      *current = *result;
      accept ();
    }
    else
    {
      if (*result > maxValue)
      {
        QString s;
        s.setNum (maxValue);
        label -> setText (s);
      }
      else
      {
        QString s;
        s.setNum (minValue);
        label -> setText (s);
      }
    }
  }
  delete result;
}

int setInt (const char *caption, int old, int min, int max)
{
  int *tmp = new int (old);
  setupInt *setup = new setupInt (tmp, min, max);
  setup -> setWindowTitle (caption);
  if (setup -> exec ())
    old = *tmp;
  delete setup;
  delete tmp;
  return old;
}

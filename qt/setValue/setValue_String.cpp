#include "setValue.h"

setupString::setupString (char *dest, const int maxlen) : QDialog (0) {
  this->setModal(true);
  setFixedSize (400, 115);

  label = new QLineEdit (this);
  label -> setGeometry (50, 20, 300, 30);
  label -> setMaxLength (maxlen - 1);
  label -> setText (dest);
  orig = dest;

  okButton = new QPushButton ("OK", this);
  okButton -> setGeometry (120, 70, 70, 30);
  connect (okButton, SIGNAL (clicked()), SLOT (accept()));
  connect (label, SIGNAL (returnPressed()), SLOT (accept()));
  cancelButton = new QPushButton ("Cancel", this);
  cancelButton -> setGeometry (210, 70, 70, 30);
  connect (cancelButton, SIGNAL (clicked()), SLOT (reject()));
}

setupString::~setupString (void)
{
  delete label;
  delete okButton;
  delete cancelButton;
}

void setupString::accept ()
{
  strcpy (orig, label -> text ().toStdString().c_str());
  QDialog::accept ();
}

char *setString (const char *caption, char *dest, const int maxlen)
{
  setupString *setup = new setupString (dest, maxlen);
  setup -> setWindowTitle (caption);
  if (setup -> exec ())
  delete setup;
  return dest;
}

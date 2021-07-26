#include "setupColor.h"

setupColor::setupColor(float *rgb, bool just) : QDialog(nullptr) {
  this->setModal(true);
  RGB = rgb;
  oldRGB [0] = RGB [0];
  oldRGB [1] = RGB [1];
  oldRGB [2] = RGB [2];

  applyJust = just;

  setFixedSize (285, 175);

  okButton = new QPushButton ("OK", this);
  okButton -> setGeometry (60, 130, 70, 30);
  connect (okButton, SIGNAL (clicked()), SLOT (accept()));
  cancelButton = new QPushButton ("Cancel", this);
  cancelButton -> setGeometry (155, 130, 70, 30);
  connect (cancelButton, SIGNAL (clicked()), SLOT (restore()));

  QString s;
  
  rText = new QLabel ("Red:", this);
  rText -> setGeometry (20, 20, 50, 20);
  rSlider = new QSlider (Qt::Orientation::Horizontal, this);
  rSlider->setMinimum(0);
  rSlider->setMaximum(100);
  rSlider->setSliderPosition((int)(100 * RGB[0]));
  rSlider->setSingleStep(1);
  rSlider -> setGeometry (85, 20, 130, 20);
  connect (rSlider, SIGNAL (valueChanged (int)), SLOT (rChanged (int)));
  s.sprintf("%1.2f", RGB [0]);
  rValue = new QLabel (s, this);
  rValue -> setGeometry (230, 20, 35, 20);

  gText = new QLabel ("Green:", this);
  gText -> setGeometry (20, 55, 50, 20);
  gSlider = new QSlider (Qt::Orientation::Horizontal, this);
  gSlider->setMinimum(0);
  gSlider->setMaximum(100);
  gSlider->setSliderPosition((int)(100 * RGB[1]));
  gSlider->setSingleStep(1);
  gSlider -> setGeometry (85, 55, 130, 20);
  connect (gSlider, SIGNAL (valueChanged (int)), SLOT (gChanged (int)));
  s.sprintf("%1.2f", RGB [1]);
  gValue = new QLabel (s, this);
  gValue -> setGeometry (230, 55, 35, 20);

  bText = new QLabel ("Blue:", this);
  bText -> setGeometry (20, 90, 50, 20);
  bSlider = new QSlider (Qt::Orientation::Horizontal, this);
  bSlider->setMinimum(0);
  bSlider->setMaximum(100);
  bSlider->setSliderPosition((int)(100 * RGB[2]));
  bSlider->setSingleStep(1);
  bSlider -> setGeometry (85, 90, 130, 20);
  connect (bSlider, SIGNAL (valueChanged (int)), SLOT (bChanged (int)));
  s.sprintf("%1.2f", RGB [2]);
  bValue = new QLabel (s, this);
  bValue -> setGeometry (230, 90, 35, 20);
}

setupColor::~setupColor (void)
{
  delete rText;
  delete gText;
  delete bText;
  delete rValue;
  delete gValue;
  delete bValue;
  delete rSlider;
  delete gSlider;
  delete bSlider;
  delete okButton;
  delete cancelButton;
}

void setupColor::restore ()
{
  bool needRestore = false;

  if (RGB [0] != oldRGB [0])
  {
    RGB [0] = oldRGB [0];
    needRestore = true;
  }
  if (RGB [1] != oldRGB [1])
  {
    RGB [1] = oldRGB [1];
    needRestore = true;
  }
  if (RGB [2] != oldRGB [2])
  {
    RGB [2] = oldRGB [2];
    needRestore = true;
  }

  if (applyJust && needRestore)
    apply ();

  reject ();
}

void setupColor::rChanged (int n)
{
  RGB [0] = n / 100.0;
  QString s;
  s.sprintf("%1.2f", RGB [0]);
  rValue -> setText (s);
  if (applyJust)
    apply ();
}

void setupColor::gChanged (int n)
{
  RGB [1] = n / 100.0;
  QString s;
  s.sprintf("%1.2f", RGB [1]);
  gValue -> setText (s);
  if (applyJust)
    apply ();
}

void setupColor::bChanged (int n)
{
  RGB [2] = n / 100.0;
  QString s;
  s.sprintf("%1.2f", RGB [2]);
  bValue -> setText (s);
  if (applyJust)
    apply ();
}

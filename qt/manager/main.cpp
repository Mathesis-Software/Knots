#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>

#include "icon_provider.h"
#include "manager.h"

int main(int argc, char **argv)
{
  QApplication qa(argc, argv);
  qa.setFont (QFont ("Helvetica", 12));

  //QFileDialog::setIconProvider (new keFileIconProvider);

  keManager kem;
  //qa.setMainWidget (&kem);
  kem.show ();
  return qa.exec ();
}

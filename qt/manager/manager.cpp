#include <fstream>
#include <stdlib.h>

#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qmessagebox.h>
#include <QtGui/qpixmap.h>

#include "manager.h"
#include "about.h"
#include "../knotWindow/knotWindow.h"
#include "../diagramWindow/diagramWindow.h"

keManager::keManager (void)
{
  fileMenu = this->addMenu("&File");
  fileMenu -> addAction ("&New diagram", this, SLOT ( new_diagram() ));
  fileMenu -> addAction ("&Open...", this, SLOT ( open() ));
  fileMenu -> addSeparator ();
  fileMenu -> addAction ("&Close all windows", this, SLOT ( close_all_windows() ));
  fileMenu -> addAction ("E&xit", this, SLOT ( exit() ));
  addSeparator ();
  aboutMenu = this->addMenu("&About");
  aboutMenu -> addAction ("About &Knot Editor", this, SLOT ( about() ));
  aboutMenu -> addAction ("About &Qt", this, SLOT ( about_qt() ));

  setWindowTitle ("Knot Editor");
  setWindowIcon (QPixmap ((QString) getenv ("KNOTEDITOR_PIXMAPS") + "/trefoil.xpm"));
  setFixedSize (300, height ());
}

keManager::~keManager ()
{
  delete fileMenu;
  delete aboutMenu;
}

void keManager::closeEvent (QCloseEvent*)
{
  if (abstractWindow::removeAll ())
    qApp -> quit ();
}

void keManager::open ()
{
  QString filename =
    QFileDialog::getOpenFileName (nullptr, "Open", getenv ("KNOTEDITOR_DATA"));

  if (filename.isEmpty ())
    return;
  
  std::ifstream is (filename.toStdString());
  if (!is)
  {
    QMessageBox::critical (0, "Error",
        "\nCouldn't open file \"" + filename + "\"\n");
    return;
  }

  QFileInfo finfo (filename);

  abstractWindow *aw = NULL;

  if (finfo.suffix () == "knt")
  {
    aw = new knotWindow (is);
    if (aw -> isEmpty ())
      QMessageBox::critical (0, "Error",
        "\nThis file is not in knot format.\n");
  }

  if (finfo.suffix () == "dgr")
  {
    aw = new diagramWindow (is);
    if (aw -> isEmpty ())
      QMessageBox::critical (0, "Error",
        "\nThis file is not in diagram format.\n");
  }

  is.close ();

  if (!aw)
  {
    QMessageBox::critical (0, "Error", "\nUnknown file type.\n");
    return;
  }

  if (aw -> isEmpty ())
    aw -> close ();
  else
    aw -> show ();
}

void keManager::new_diagram ()
{
  abstractWindow *aw = new diagramWindow ();
  aw -> show ();
}

void keManager::about ()
{
  aboutWindow *aw = new aboutWindow (0);
  aw -> show ();
}

void keManager::about_qt ()
{
  QMessageBox::aboutQt (0);
}

void keManager::exit ()
{
  QWidget::close ();
}

void keManager::close_all_windows ()
{
  abstractWindow::removeAll ();
}

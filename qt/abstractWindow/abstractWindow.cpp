#include <stdlib.h>

#include <QtWidgets/qfiledialog.h>
#include <QtWidgets/qmenubar.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qmenu.h>
//#include <QtWidgets/qprinter.h>
#include <QtWidgets/qstatusbar.h>
#include <QtWidgets/qtoolbar.h>
#include <QtWidgets/qtoolbutton.h>
#include <QtWidgets/qwhatsthis.h>

#include "abstractWindow.h"
#include "../setValue/setValue.h"

std::list<abstractWindow*> abstractWindow::AWRegister;

abstractWindow::abstractWindow() {
  this->setAttribute(Qt::WA_DeleteOnClose);

  isSaved = true;

  QMenu *fileMenu = this->menuBar()->addMenu("&File");

  fileMenu -> addAction ( "&Save as...", this, SLOT (save_as()) );
  fileMenu -> addAction ( "&Print...", this, SLOT (print()) );
  fileMenu -> addSeparator ();
  fileMenu -> addAction ( "&Rename...", this, SLOT (rename()) );
  fileMenu -> addSeparator ();
  fileMenu -> addAction ( "&Close", this, SLOT (close()) );

  ToolBar = new QToolBar (this);

  addToolBarButton ("floppy.xpm", "Save as", SLOT (save_as()));
  addToolBarButton ("printer.xpm", "Print", SLOT (print()));

  AWRegister.push_back(this);
}

abstractWindow::~abstractWindow (void)
{
  delete ToolBar;
}

int abstractWindow::askForSave (void)
{
  show ();
  raise ();
  while (!isSaved)
  {
    QString q = (QString) "\nSave \"" + this->windowTitle () + "\" before closing?\n";
    int answer = QMessageBox::warning (this,
                                       "Close",
				       q.toStdString().c_str(),
				       "&Yes",
				       "&No",
				       "&Cancel");
    if (answer)
      return answer - 1;

    save_as ();
  }
  return 0;
}

void abstractWindow::closeEvent (QCloseEvent*)
{
  if ( (!isSaved) && (askForSave ()) )
    return;

  AWRegister.remove(this);
}

void abstractWindow::save_as ()
{
  if (isEmpty ())
    return;

  QString filename = QFileDialog::getSaveFileName (nullptr, "Save", getenv ("KNOTEDITOR_DATA"), mask ());
  if (filename.isEmpty ())
    return;
  
  {
    std::ifstream is (filename.toStdString());
    if (is) {
      is.close ();
      if ( QMessageBox::warning (this, "Save",
               "\nFile " + filename + " already exist. Overwrite?\n",
               "Yes", "No", 0, 1) )
	return;
    }
  }

  std::ofstream os (filename.toStdString());
  if (!os) {
    QMessageBox::critical (this, "Error",
        "\nCouldn't open file \"" + filename + "\"\n");
    return;
  }

  saveIt (os);
  os.close ();
  statusBar () -> showMessage ("File saved.", 3000);
  isSaved = true;
}

void abstractWindow::print ()
{
  /*QPrinter prn;
  if (prn.setup (this))
    printIt (&prn);*/
}

void abstractWindow::rename ()
{
  char *capt = new char [256];
  strcpy (capt, windowTitle ().toStdString().c_str());
  setString ("Rename", capt, 256);
  if ( strcmp (capt, windowTitle ().toStdString().c_str()) )
  {
    setWindowTitle (capt);
    isSaved = false;
  }
  delete[] capt;
}

void abstractWindow::close ()
{
  QWidget::close ();
}
  
bool abstractWindow::removeAll (void)
{
  while (!abstractWindow::AWRegister.empty()) {
    abstractWindow *av = abstractWindow::AWRegister.back();
    if ( (!av -> isSaved) && (av -> askForSave ()) )
      return false;
    abstractWindow::AWRegister.pop_back();
    delete av;
  }
  return true;
}

QToolButton *abstractWindow::addToolBarButton (const char *filename,
                                               const char *label,
			                       const char *slot,
					       const char *whatsthis)
{
  QPixmap pm =
    QPixmap ((QString) getenv ("KNOTEDITOR_PIXMAPS") + "/" + filename);
  QToolButton *qtb = new QToolButton (ToolBar);
  qtb->setIcon(pm);
  qtb->setText(label);
  //QToolButton *qtb = new QToolButton (pm, label, 0, this, slot, ToolBar);
  //qtb -> setUsesBigPixmap (true);
  qtb -> setFixedSize (pm.width () + 6, 30);
  //if (whatsthis)
  //  QWhatsThis::add (qtb, whatsthis);
  return qtb;
}

void abstractWindow::addToolBarSeparator (void)
{
  ToolBar -> addSeparator ();
}

void abstractWindow::complete (bool showWhatsThis)
{
  if (showWhatsThis)
  {
    addToolBarSeparator ();
    //QWhatsThis::whatsThisButton (ToolBar) -> setFixedSize (30, 30);
  }
  ToolBar -> show ();
  statusBar () -> isVisible ();
  resize (508, 594);
}

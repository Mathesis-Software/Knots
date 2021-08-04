#include <fstream>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPixmap>

#include "manager.h"
#include "about.h"
#include "icon_provider.h"
#include "../knotWindow/knotWindow.h"
#include "../diagramWindow/diagramWindow.h"

keManager::keManager() {
  fileMenu = this->addMenu("&File");
  fileMenu->addAction("&New diagram", this, SLOT(new_diagram()));
  fileMenu->addAction("&Open...", this, SLOT(open()));
  fileMenu->addSeparator();
  fileMenu->addAction("&Close all windows", this, SLOT(close_all_windows()));
  fileMenu->addAction("E&xit", this, SLOT(exit()));
  addSeparator();
  aboutMenu = this->addMenu("&About");
  aboutMenu->addAction("About &Knot Editor", this, SLOT(about()));
  aboutMenu->addAction("About &Qt", this, SLOT(about_qt()));

  setWindowTitle("Knot Editor");
  setWindowIcon(QPixmap((QString)getenv("KNOTEDITOR_PIXMAPS") + "/trefoil.xpm"));
  setFixedSize(300, height());
}

keManager::~keManager() {
  delete fileMenu;
  delete aboutMenu;
}

void keManager::closeEvent(QCloseEvent*) {
  if (abstractWindow::removeAll()) {
    qApp->quit();
	}
}

namespace {

QString getOpenFileNameEx() {
  QFileDialog dialog(nullptr, "Open file", getenv("KNOTEDITOR_DATA"));
  dialog.setSupportedSchemes(QStringList(QStringLiteral("file")));
  dialog.setIconProvider(keFileIconProvider::instance());
  if (dialog.exec() == QDialog::Accepted) {
    return dialog.selectedUrls().value(0).toLocalFile();
  }
  return QString();
}

}

void keManager::open() {
  QString filename = getOpenFileNameEx();

  if (filename.isEmpty()) {
    return;
  }
  
  std::ifstream is(filename.toStdString());
  if (!is) {
    QMessageBox::critical(0, "Error", "\nCouldn't open file \"" + filename + "\"\n");
    return;
  }

  QFileInfo finfo(filename);

  abstractWindow *aw = nullptr;

  if (finfo.suffix() == "knt") {
		try {
    	aw = new knotWindow(is);
		} catch (const std::runtime_error &e) {
			abstractWindow::AWRegister.pop_back();
      QMessageBox::critical(0, "Knot opening error", QString("\n") + e.what() + "\n");
    	return;
    }
  } else if (finfo.suffix() == "dgr") {
		try {
			aw = new diagramWindow(is);
		} catch (const std::runtime_error &e) {
			abstractWindow::AWRegister.pop_back();
      QMessageBox::critical(0, "Diagram opening error", QString("\n") + e.what() + "\n");
    	return;
    }
  } else {
    QMessageBox::critical(0, "File opening error", "\nUnknown file type.\n");
    return;
  }

  is.close();

  if (aw->isEmpty()) {
    aw->close();
  } else {
    aw->show();
  }
}

void keManager::new_diagram() {
  abstractWindow *aw = new diagramWindow();
  aw->show();
}

void keManager::about() {
  aboutWindow *aw = new aboutWindow(0);
  aw->show();
}

void keManager::about_qt() {
  QMessageBox::aboutQt(0);
}

void keManager::exit() {
  QWidget::close();
}

void keManager::close_all_windows() {
  abstractWindow::removeAll();
}

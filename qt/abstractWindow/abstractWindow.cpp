#include <cstdlib>

#include <QtGui/QCloseEvent>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QMenu>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWhatsThis>

#include "abstractWindow.h"
#include "../setValue/setValue.h"

std::list<abstractWindow*> abstractWindow::AWRegister;

abstractWindow::abstractWindow() {
	this->setAttribute(Qt::WA_DeleteOnClose);

	isSaved = true;

	QMenu *fileMenu = this->menuBar()->addMenu("&File");

	fileMenu->addAction("&Save as…", this, SLOT(save_as()));
	fileMenu->addAction("&Print…", this, SLOT(print()));
	fileMenu->addSeparator();
	fileMenu->addAction("&Rename…", this, SLOT(rename()));
	fileMenu->addSeparator();
	fileMenu->addAction("&Close", this, SLOT(close()));

	this->toolbar = new QToolBar(this);
	addToolBar(this->toolbar);

	AWRegister.push_back(this);
}

abstractWindow::~abstractWindow() {
	delete this->toolbar;
}

int abstractWindow::askForSave() {
	show();
	raise();
	while (!isSaved) {
		QString q = "\nSave \"" + this->windowTitle() + "\" before closing?\n";
		int answer = QMessageBox::warning(
			this, "Close", q.toStdString().c_str(), "&Yes", "&No", "&Cancel"
		);
		if (answer)
			return answer - 1;

		save_as();
	}
	return 0;
}

void abstractWindow::closeEvent(QCloseEvent *event) {
	if (!isSaved && askForSave()) {
		event->ignore();
		return;
	}

	AWRegister.remove(this);
}

void abstractWindow::save_as() {
	if (isEmpty()) {
		return;
	}

	QString filename = QFileDialog::getSaveFileName(nullptr, "Save", getenv("KNOTEDITOR_DATA"), mask());
	if (filename.isEmpty())
		return;

	std::ofstream os(filename.toStdString());
	if (!os) {
		QMessageBox::critical(this, "Error", "\nCouldn't open file \"" + filename + "\"\n");
		return;
	}

	saveIt(os);
	os.close();
	statusBar()->showMessage("File saved.", 3000);
	isSaved = true;
}

void abstractWindow::print() {
	QPrinter prn;
	//if (prn.setup(this)) {
	//	printIt(&prn);
	//}
}

void abstractWindow::rename() {
	char *capt = new char [256];
	strcpy(capt, windowTitle().toStdString().c_str());
	setString("Rename", capt, 256);
	if (strcmp(capt, windowTitle().toStdString().c_str())) {
		setWindowTitle(capt);
		isSaved = false;
	}
	delete[] capt;
}

bool abstractWindow::closeAllWindows() {
	while (!abstractWindow::AWRegister.empty()) {
		abstractWindow *av = abstractWindow::AWRegister.back();
		if (!av->close()) {
			return false;
		}
	}
	return true;
}

QAction *abstractWindow::addToolbarAction(const QString &iconFilename, const QString &text, const std::function<void()> &functor) {
	const QIcon icon((QString)getenv ("KNOTEDITOR_PIXMAPS") + "/" + iconFilename);
	return this->toolbar->addAction(icon, text, functor);
}

void abstractWindow::addToolBarSeparator() {
	this->toolbar->addSeparator();
}

void abstractWindow::complete() {
	this->toolbar->show();
	statusBar()->isVisible();
	resize(508, 594);
}

void abstractWindow::registerAction(QAction *action, std::function<void(QAction&)> controller) {
	this->actionsMap[action] = controller;
}

void abstractWindow::updateActions() {
	for (auto &[action, controller] : this->actionsMap) {
		controller(*action);
	}
}

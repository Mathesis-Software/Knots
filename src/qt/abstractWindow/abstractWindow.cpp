#include <cstdlib>

#include <QtGui/QCloseEvent>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QInputDialog>
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

	QMenu *fileMenu = this->menuBar()->addMenu("&File");

	fileMenu->addAction("&Save as…", [this] { this->save(); });
	fileMenu->addAction("&Print…", [this] { this->print(); });
	fileMenu->addSeparator();
	fileMenu->addAction("Rename…", [this] { this->rename(); });
	fileMenu->addSeparator();
	fileMenu->addAction("Close", [this] { this->close(); });

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
	while (!this->isSaved()) {
		QString q = "\nSave \"" + this->windowTitle() + "\" before closing?\n";
		int answer = QMessageBox::warning(
			this, "Close", q.toStdString().c_str(), "&Yes", "&No", "&Cancel"
		);
		if (answer)
			return answer - 1;

		this->save();
	}
	return 0;
}

void abstractWindow::closeEvent(QCloseEvent *event) {
	if (!this->isSaved() && this->askForSave()) {
		event->ignore();
		return;
	}

	AWRegister.remove(this);
}

void abstractWindow::save() {
	QString filename = QFileDialog::getSaveFileName(nullptr, "Save", getenv("KNOTEDITOR_DATA"), mask());
	if (filename.isEmpty()) {
		return;
	}

	std::ofstream os(filename.toStdString());
	if (!os) {
		QMessageBox::critical(this, "Error", "\nCouldn't open file \"" + filename + "\" for writing\n");
		return;
	}

	saveIt(os);
	os.close();
}

void abstractWindow::print() {
	QPrinter prn;
	//if (prn.setup(this)) {
	//	printIt(&prn);
	//}
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
	const QIcon icon(":images/" + iconFilename);
	return this->toolbar->addAction(icon, text, functor);
}

void abstractWindow::addToolbarSeparator() {
	this->toolbar->addSeparator();
}

void abstractWindow::complete() {
	this->toolbar->show();
	statusBar()->setVisible(true);
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

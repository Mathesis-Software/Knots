#include <cstdlib>

#include <QtGui/QCloseEvent>
#include <QtWidgets/QApplication>
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
#include "../manager/about.h"
#include "../manager/iconProvider.h"
#include "../manager/manager.h"

std::list<abstractWindow*> abstractWindow::AWRegister;

abstractWindow::abstractWindow() {
	this->setAttribute(Qt::WA_DeleteOnClose);

	QMenu *fileMenu = this->menuBar()->addMenu("File");

	auto newd = fileMenu->addAction("New diagram", [this] { KE::Qt::ManagerWindow::newDiagram(); });
	newd->setShortcut(QKeySequence("Ctrl+N"));
	auto open = fileMenu->addAction("Open…", [this] { KE::Qt::ManagerWindow::openFile(); });
	open->setShortcut(QKeySequence("Ctrl+O"));
	fileMenu->addSeparator();
	auto save = fileMenu->addAction("Save as…", [this] { this->save(); });
	save->setShortcut(QKeySequence("Ctrl+S"));
	fileMenu->addAction("Print…", [this] { this->print(); })->setEnabled(false);
	fileMenu->addSeparator();
	fileMenu->addAction("Rename…", [this] { this->rename(); });
	fileMenu->addSeparator();
	fileMenu->addAction("About", [] { KE::Qt::AboutWindow::showAboutDialog(); });
	fileMenu->addSeparator();
	auto close = fileMenu->addAction("Close", [this] { this->close(); });
	close->setShortcut(QKeySequence("Ctrl+W"));
	auto quit = fileMenu->addAction("Quit", [this] { this->exit(); });
	quit->setShortcut(QKeySequence("Ctrl+Q"));

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

namespace {

QString getSaveFileNameEx(const QString &fileFilter) {
	QFileDialog dialog(nullptr, "Save file", getenv("KNOTEDITOR_DATA"));
	dialog.setSupportedSchemes(QStringList(QStringLiteral("file")));
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.setIconProvider(KE::Qt::FileIconProvider::instance());
	dialog.setNameFilters({
		fileFilter,
		"Any files (*)"
	});
	if (dialog.exec() == QDialog::Accepted) {
		return dialog.selectedUrls().value(0).toLocalFile();
	}
	return QString();
}

}

void abstractWindow::save() {
	QString filename = getSaveFileNameEx(this->fileFilter());
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

void abstractWindow::exit() {
	if (closeAllWindows()) {
		qApp->quit();
	}
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
	this->resize(508, 594);
}

QAction *abstractWindow::registerAction(QAction *action, std::function<void(QAction&)> controller) {
	this->actionsMap[action] = controller;
	return action;
}

void abstractWindow::updateActions() {
	for (auto &[action, controller] : this->actionsMap) {
		controller(*action);
	}
}

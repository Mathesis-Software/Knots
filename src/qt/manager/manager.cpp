#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>

#include <rapidjson/istreamwrapper.h>

#include "../../util/rapidjson.h"
#include "manager.h"
#include "about.h"
#include "iconProvider.h"
#include "../knot/knotWindow.h"
#include "../diagram/diagramWindow.h"

namespace KE { namespace Qt {

ManagerWindow::ManagerWindow() {
	fileMenu = this->menuBar()->addMenu("File");
	fileMenu->addAction("About", [this] { this->about(); });
	fileMenu->addSeparator();
	fileMenu->addAction("Close all windows", [this] { this->close_all_windows(); });
	fileMenu->addAction("Quit", [this] { this->exit(); });

	auto center = new QWidget;
	auto layout = new QGridLayout(center);
	{
		auto button = new QPushButton("Create new diagram");
		button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		button->connect(button, &QPushButton::pressed, [this] { this->new_diagram(); });
		layout->addWidget(button, 0, 0);
	}
	{
		auto button = new QPushButton("Open existing file");
		button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		button->connect(button, &QPushButton::pressed, [this] { this->open(); });
		layout->addWidget(button, 0, 1);
	}
	this->setCentralWidget(center);

	setWindowTitle("Knot Editor");
	setFixedSize(500, 200);
}

ManagerWindow::~ManagerWindow() {
	delete fileMenu;
}

void ManagerWindow::closeEvent(QCloseEvent*) {
	if (abstractWindow::closeAllWindows()) {
		qApp->quit();
	}
}

namespace {

QString getOpenFileNameEx() {
	QFileDialog dialog(nullptr, "Open file", getenv("KNOTEDITOR_DATA"));
	dialog.setSupportedSchemes(QStringList(QStringLiteral("file")));
	dialog.setIconProvider(FileIconProvider::instance());
	dialog.setNameFilters({
		"Knot Editor files (*.knt *.dgr)",
		"Knot files only (*.knt)",
		"Diagram files only (*.dgr)",
		"Any files (*)"
	});
	if (dialog.exec() == QDialog::Accepted) {
		return dialog.selectedUrls().value(0).toLocalFile();
	}
	return QString();
}

}

void ManagerWindow::open() {
	QString filename = getOpenFileNameEx();

	if (filename.isEmpty()) {
		return;
	}

	abstractWindow *aw = nullptr;

	try {
		std::ifstream is(filename.toStdString());
		if (!is) {
			throw std::runtime_error("Cannot read the file content");
		}
		rapidjson::Document doc;
		rapidjson::IStreamWrapper wrapper(is);
		doc.ParseStream(wrapper);
		is.close();

		if (doc.IsNull()) {
			throw std::runtime_error("The file is not in JSON format");
		} else if (doc.IsObject() && Util::rapidjson::get_string(doc, "type") == "diagram") {
			aw = new diagramWindow(doc);
		} else if (doc.IsObject() && Util::rapidjson::get_string(doc, "type") == "link") {
			aw = new knotWindow(doc);
		} else {
			throw std::runtime_error("The file does not represent a knot nor a diagram");
		}

		if (aw->isEmpty()) {
			aw->close();
		} else {
			aw->show();
		}
	} catch (const std::runtime_error &e) {
		abstractWindow::AWRegister.pop_back();
		QMessageBox::critical(0, "File opening error", QString("\n") + e.what() + "\n");
	}
}

void ManagerWindow::new_diagram() {
	abstractWindow *aw = new diagramWindow();
	aw->show();
}

void ManagerWindow::about() {
	auto aw = new AboutWindow(0);
	aw->showMe();
}

void ManagerWindow::exit() {
	QWidget::close();
}

void ManagerWindow::close_all_windows() {
	abstractWindow::closeAllWindows();
}

}}

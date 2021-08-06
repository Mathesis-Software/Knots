#include <fstream>

#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtGui/QPixmap>

#include <rapidjson/istreamwrapper.h>

#include "../../util/rapidjson.h"
#include "manager.h"
#include "about.h"
#include "icon_provider.h"
#include "../knotWindow/knotWindow.h"
#include "../diagram/diagramWindow.h"

keManager::keManager() {
	fileMenu = this->addMenu("&File");
	fileMenu->addAction("&New diagram", this, SLOT(new_diagram()));
	fileMenu->addAction("&Open…", this, SLOT(open()));
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

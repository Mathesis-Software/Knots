/*
 * Copyright (c) 1995-2021, Nikolay Pultsin <geometer@geometer.name>
 *
 * Licensed under the Apache License, Version 2.0 the "License";
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fstream>

#include <QtCore/QResource>
#include <QtCore/QSettings>
#include <QtCore/QStandardPaths>
#include <QtGui/QFileOpenEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QProxyStyle>

#include <rapidjson/istreamwrapper.h>

#include "Application.h"
#include "DiagramWindow.h"
#include "FileIconProvider.h"
#include "KnotWindow.h"
#include "LibraryWindow.h"
#include "StartWindow.h"
#include "../ke/Util_rapidjson.h"

namespace KE::Qt {

namespace {

class ProxyStyle : public QProxyStyle {

QPixmap generatedIconPixmap(QIcon::Mode iconMode, const QPixmap &pixmap, const QStyleOption *option) const override {
	if (iconMode == QIcon::Disabled) {
		QPixmap copy(pixmap);
		QPainter painter(&copy);
		painter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
		painter.fillRect(copy.rect(), 0xc0c0c0);
		painter.end();
		return copy;
	}
	return QProxyStyle::generatedIconPixmap(iconMode, pixmap, option);
}

};

}

void Application::closeStartWindow() {
	for (auto widget : QApplication::topLevelWidgets()) {
		if (auto window = dynamic_cast<StartWindow*>(widget)) {
			window->close();
		}
	}
}

QWidget *Application::library() {
	for (auto widget : QApplication::topLevelWidgets()) {
		if (auto window = dynamic_cast<LibraryWindow*>(widget)) {
			window->showNormal();
			window->raise();
			window->activateWindow();
			return window;
		}
	}

	auto window = new LibraryWindow();
	window->show();
	window->raise();
	window->activateWindow();
	this->closeStartWindow();
	return window;
}

QWidget *Application::newDiagram() {
	auto window = new DiagramWindow();
	window->show();
	this->closeStartWindow();
	return window;
}

void Application::diagramFromCode() {
	bool ok;
	QString code = QInputDialog::getText(
		nullptr, "Creating diagram from code", "Code:", QLineEdit::Normal, QString(), &ok
	);
	if (ok) {
		this->diagramFromCode(code);
	}
}

namespace {

QString getOpenFileNameEx() {
	QSettings settings;
	QString dir = settings.value("CustomFilesFolder").toString();
	if (dir.isEmpty()) {
		dir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
	}
	QFileDialog dialog(nullptr, "Open file", dir);
	dialog.setSupportedSchemes(QStringList(QStringLiteral("file")));
	dialog.setIconProvider(FileIconProvider::instance());
	dialog.setNameFilters({
		"Knot Editor files (*.knt *.dgr)",
		"Knot files only (*.knt)",
		"Diagram files only (*.dgr)",
		"Any files (*)"
	});
	if (dialog.exec() == QDialog::Accepted) {
		settings.setValue("CustomFilesFolder", dialog.directory().path());
		settings.sync();
		return dialog.selectedUrls().value(0).toLocalFile();
	}
	return QString();
}

}

QWidget *Application::openFile() {
	return openFile(getOpenFileNameEx());
}

QWidget *Application::openFile(const QString &filename) {
	if (filename.isEmpty()) {
		return nullptr;
	}

	try {
		rapidjson::Document doc;

		if (filename.startsWith(":")) {
			QResource resource(filename);
			if (!resource.isValid() || resource.data() == nullptr || resource.size() == 0) {
				throw std::runtime_error("Cannot read the resource content");
			}
			doc.Parse(reinterpret_cast<const char*>(resource.data()), resource.size());
		} else {
			std::ifstream is(filename.toStdString());
			if (!is) {
				throw std::runtime_error("Cannot read the file content");
			}
			rapidjson::IStreamWrapper wrapper(is);
			doc.ParseStream(wrapper);
			is.close();
		}

		Window *window = nullptr;
		if (doc.IsNull()) {
			throw std::runtime_error("The file is not in JSON format");
		} else if (doc.IsObject() && Util::rapidjson::getString(doc, "type") == "diagram") {
			window = new DiagramWindow(doc, filename);
		} else if (doc.IsObject() && Util::rapidjson::getString(doc, "type") == "link") {
			window = new KnotWindow(doc, filename);
		} else {
			throw std::runtime_error("The file does not represent a knot nor a diagram");
		}

		window->show();
		this->closeStartWindow();
		return window;
	} catch (const std::runtime_error &e) {
		QMessageBox::critical(nullptr, "File opening error", QString("\n") + e.what() + "\n");
		return nullptr;
	}
}

Application::Application(int &argc, char **argv) : QApplication(argc, argv), windowsListSaved(false) {
	this->setFont(QFont("Helvetica", 10));
	this->setStyle(new ProxyStyle);

	QObject::connect(this, &QApplication::aboutToQuit, [this] {
		if (!this->windowsListSaved) {
			QSettings settings;
			settings.setValue("OpenWindows", QStringList());
			settings.sync();
		}
	});

	QPixmap pixmap(":images/trefoil.png");
	pixmap.setDevicePixelRatio(this->devicePixelRatio());
	this->setWindowIcon(pixmap);

	int count = 0;
	for (int i = 1; i < argc; ++i) {
		if (auto window = Application::openFile(argv[i])) {
			window->raise();
			window->activateWindow();
			count += 1;
		}
	}
	if (count == 0) {
		QSettings settings;
		auto ids = settings.value("OpenWindows").toStringList();
		if (ids.isEmpty()) {
			const auto last = settings.value("LastClosedWindow").toString();
			if (!last.isEmpty()) {
				ids.append(last);
			}
		}
		for (const auto &id : ids) {
			if (id == "::LIBRARY::") {
				Application::library();
				count += 1;
			} else if (auto window = Application::openFile(id)) {
				window->raise();
				window->activateWindow();
				count += 1;
			}
		}
	}

	if (count == 0) {
		(new StartWindow())->show();
	}
}

bool Application::event(QEvent *event) {
	if (event->type() == QEvent::FileOpen) {
		const auto file = static_cast<QFileOpenEvent*>(event)->file();
		if (this->openFile(file) == nullptr) {
			this->closeStartWindow();
		}
		return true;
	} else {
		return QApplication::event(event);
	}
}

void Application::exitApplication() {
	QStringList ids;
	for (auto widget : QApplication::topLevelWidgets()) {
		if (auto window = dynamic_cast<BaseWindow*>(widget)) {
			if (window->close()) {
				const auto id = window->identifier();
				if (!id.isNull()) {
					ids.append(id);
				}
			} else {
				return;
			}
		}
	}

	QSettings settings;
	settings.setValue("OpenWindows", ids);
	settings.sync();
	this->windowsListSaved = true;
	this->quit();
}

}

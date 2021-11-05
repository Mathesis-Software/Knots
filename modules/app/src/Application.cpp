/*
 * Copyright (c) 1995-2021, Mathesis Software <mad@mathesis.fun>
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

#include <Util_rapidjson.h>

#include "Application.h"
#include "DiagramWindow.h"
#include "FileIconProvider.h"
#include "IPCController.h"
#include "KnotWindow.h"
#include "LibraryWindow.h"

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

QWidget *Application::openLibrary() {
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
	return window;
}

QWidget *Application::newDiagram() {
	auto window = new DiagramWindow();
	window->show();
	return window;
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
	dialog.setNameFilters({"Mathesis Knots files (*.knt *.dgr)",
												 "Knot files only (*.knt)",
												 "Diagram files only (*.dgr)",
												 "Any files (*)"});
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

	for (auto widget : QApplication::topLevelWidgets()) {
		if (auto window = dynamic_cast<Window *>(widget)) {
			if (window->identifier() == QFileInfo(filename).canonicalFilePath()) {
				window->showNormal();
				window->raise();
				window->activateWindow();
				return window;
			}
		}
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

		return this->openDocument(doc, filename);
	} catch (const std::runtime_error &e) {
		QMessageBox::warning(nullptr, "File opening error", QString("\n") + e.what() + "\n");
		return nullptr;
	}
}

QWidget *Application::openDocument(const rapidjson::Value &doc, const QString &identifier) {
	Window *window = nullptr;
	if (doc.IsNull()) {
		throw std::runtime_error("The data are not in JSON format");
	} else if (Util::rapidjson::getString(doc, "type") == "diagram") {
		window = new DiagramWindow(doc, identifier);
	} else if (Util::rapidjson::getString(doc, "type") == "link") {
		window = new KnotWindow(doc, identifier);
	} else {
		throw std::runtime_error("The data do not represent a knot nor a diagram");
	}

	window->show();
	return window;
}

Application::Application(int &argc, char **argv) : QApplication(argc, argv), windowsListSaved(false) {
	this->ipcController = new IPC::Controller("fun.mathesis.Knots", this);
	switch (this->ipcController->role()) {
		case IPC::Controller::Role::primary:
			QObject::connect(this->ipcController, &IPC::Controller::messageReceived, this, [this](const QStringList &messageList) {
				for (const auto &arg : messageList) {
					if (arg == "-L") {
						this->openLibrary();
					} else if (arg == "-N") {
						this->newDiagram();
					} else if (arg.startsWith("-")) {
						qDebug() << QString("Unknown command line key %1").arg(arg);
					} else {
						this->openFile(arg);
					}
				}
			});
			break;
		case IPC::Controller::Role::secondary:
			this->ipcController->sendMessage(this->arguments().mid(1));
			return;
		case IPC::Controller::Role::unknown:
			// unexpected error; should never happen
			return;
	}

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
	for (const auto &arg : this->arguments().mid(1)) {
		if (arg == "-L") {
			this->openLibrary();
			count += 1;
		} else if (arg == "-N") {
			this->newDiagram();
			count += 1;
		} else {
			if (auto window = this->openFile(arg)) {
				count += 1;
			}
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
				this->openLibrary();
				count += 1;
			} else if (auto window = this->openFile(id)) {
				count += 1;
			}
		}
	}

	if (count == 0) {
		this->openLibrary();
	}
}

bool Application::doNotRun() {
	return this->ipcController->role() != IPC::Controller::Role::primary;
}

bool Application::event(QEvent *event) {
	if (event->type() == QEvent::FileOpen) {
		const auto file = static_cast<QFileOpenEvent*>(event)->file();
		this->openFile(file);
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

/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at

 *   http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 * Author: Nikolay Pultsin <geometer@geometer.name>
 */

#include <fstream>

#include <QtCore/QResource>
#include <QtCore/QSettings>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>

#include <rapidjson/istreamwrapper.h>

#include "AboutWindow.h"
#include "DiagramWindow.h"
#include "FileIconProvider.h"
#include "KnotWindow.h"
#include "LibraryWindow.h"
#include "Window.h"
#include "../ke/Util_rapidjson.h"

namespace KE::Qt {

void BaseWindow::exitApplication() {
	QStringList names;
	QMap<QString,QRect> geometries;
	for (auto widget : QApplication::topLevelWidgets()) {
		if (auto window = dynamic_cast<Window*>(widget)) {
			const auto geometry = window->geometry();
			if (window->close()) {
				const auto filename = window->filename();
				if (!filename.isNull()) {
					names.append(QFileInfo(filename).canonicalFilePath());
				}
				geometries[filename] = geometry;
			} else {
				return;
			}
		} else if (auto window = dynamic_cast<LibraryWindow*>(widget)) {
			names.append("::LIBRARY::");
			geometries["::LIBRARY::"] = window->geometry();
			window->close();
		}
	}

	QSettings settings;
	settings.setValue("OpenWindows", names);
	for (auto iter = geometries.begin(); iter != geometries.end(); ++iter) {
		settings.beginGroup("Window:" + iter.key());
		settings.setValue("geometry", iter.value());
		settings.endGroup();
	}
	settings.sync();

	qApp->quit();
}

namespace {

QString getOpenFileNameEx() {
	QFileDialog dialog(nullptr, "Open file", getenv("KNOTEDITOR_DATA"));
	dialog.setSupportedSchemes(QStringList(QStringLiteral("file")));
	dialog.setIconProvider(Qt::FileIconProvider::instance());
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

QWidget *BaseWindow::library() {
	for (auto widget : QApplication::topLevelWidgets()) {
		if (auto window = dynamic_cast<LibraryWindow*>(widget)) {
			window->showNormal();
			window->raise();
			return window;
		}
	}

	auto window = new LibraryWindow();
	window->show();
	return window;
}

QWidget *BaseWindow::newDiagram() {
	auto window = new DiagramWindow();
	window->show();
	return window;
}

QWidget *BaseWindow::openFile() {
	return openFile(getOpenFileNameEx());
}

QWidget *BaseWindow::openFile(const QString &filename) {
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
		return window;
	} catch (const std::runtime_error &e) {
		QMessageBox::critical(nullptr, "File opening error", QString("\n") + e.what() + "\n");
		return nullptr;
	}
}

BaseWindow::BaseWindow() {
	this->setAttribute(::Qt::WA_DeleteOnClose);
	this->menuBar()->setContextMenuPolicy(::Qt::PreventContextMenu);
}

void BaseWindow::createFileMenu() {
	Window *window = dynamic_cast<Window*>(this);

	QMenu *fileMenu = this->menuBar()->addMenu("File");

	auto library = fileMenu->addAction("Library", [] { Window::library(); });
	library->setShortcut(QKeySequence("Ctrl+L"));
	auto newd = fileMenu->addAction("New diagram", [] { Window::newDiagram(); });
	newd->setShortcut(QKeySequence("Ctrl+N"));
	auto open = fileMenu->addAction("Open…", [] { Window::openFile(); });
	open->setShortcut(QKeySequence("Ctrl+O"));
	fileMenu->addSeparator();
	if (window) {
		auto save = fileMenu->addAction("Save as…", window, &Window::save);
		save->setShortcut(QKeySequence("Ctrl+S"));
		fileMenu->addAction("Export as image…", window, &Window::exportPNG);
		fileMenu->addSeparator();
		fileMenu->addAction("Rename…", window, &Window::rename);
		fileMenu->addSeparator();
	}
	fileMenu->addAction("About", [] { Qt::AboutWindow::showAboutDialog(); });
	fileMenu->addSeparator();
	auto close = fileMenu->addAction("Close", [this] { this->close(); });
	close->setShortcut(QKeySequence("Ctrl+W"));
	auto quit = fileMenu->addAction("Quit", [] { Window::exitApplication(); });
	quit->setShortcut(QKeySequence("Ctrl+Q"));
}

}

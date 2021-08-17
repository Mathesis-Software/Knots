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

#include <QtWidgets/QApplication>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

#include <rapidjson/istreamwrapper.h>

#include "Window.h"
#include "DiagramWindow.h"
#include "KnotWindow.h"
#include "../manager/iconProvider.h"
#include "../../util/rapidjson.h"

namespace KE { namespace Qt {

std::list<Window*> Window::AWRegister;

void Window::exitApplication() {
	if (closeAllWindows()) {
		qApp->quit();
	}
}

bool Window::closeAllWindows() {
	while (!Window::AWRegister.empty()) {
		Window *av = Window::AWRegister.back();
		if (!av->close()) {
			return false;
		}
	}
	return true;
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

QWidget *Window::newDiagram() {
	auto window = new DiagramWindow();
	window->show();
	return window;
}

QWidget *Window::openFile() {
	QString filename = getOpenFileNameEx();

	if (filename.isEmpty()) {
		return nullptr;
	}

	try {
		std::ifstream is(filename.toStdString());
		if (!is) {
			throw std::runtime_error("Cannot read the file content");
		}
		rapidjson::Document doc;
		rapidjson::IStreamWrapper wrapper(is);
		doc.ParseStream(wrapper);
		is.close();

		Window *window = nullptr;
		if (doc.IsNull()) {
			throw std::runtime_error("The file is not in JSON format");
		} else if (doc.IsObject() && Util::rapidjson::getString(doc, "type") == "diagram") {
			window = new DiagramWindow(doc);
		} else if (doc.IsObject() && Util::rapidjson::getString(doc, "type") == "link") {
			window = new KnotWindow(doc);
		} else {
			throw std::runtime_error("The file does not represent a knot nor a diagram");
		}

		window->show();
		return window;
	} catch (const std::runtime_error &e) {
		Window::AWRegister.pop_back();
		QMessageBox::critical(0, "File opening error", QString("\n") + e.what() + "\n");
		return nullptr;
	}
}

}}

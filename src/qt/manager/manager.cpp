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
	fileMenu->addAction("About", [] { AboutWindow::showAboutDialog(); });
	fileMenu->addSeparator();
	auto quit = fileMenu->addAction("Quit", [] { abstractWindow::exitApplication(); });
	quit->setShortcut(QKeySequence("Ctrl+Q"));

	auto center = new QWidget;
	auto layout = new QGridLayout(center);
	{
		auto button = new QPushButton("Create new diagram");
		button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		button->connect(button, &QPushButton::clicked, [this] {
			auto window = newDiagram();
			window->setGeometry(this->geometry());
			this->close();
		});
		layout->addWidget(button, 0, 0);
	}
	{
		auto button = new QPushButton("Open existing file");
		button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		button->connect(button, &QPushButton::clicked, [this] {
			auto window = openFile();
			if (window) {
				window->setGeometry(this->geometry());
				this->close();
			}
		});
		layout->addWidget(button, 0, 1);
	}
	this->setCentralWidget(center);

	setWindowTitle("Knot Editor");
	this->resize(508, 594);
}

ManagerWindow::~ManagerWindow() {
	delete fileMenu;
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

QWidget *ManagerWindow::newDiagram() {
	auto window = new diagramWindow();
	window->show();
	return window;
}

QWidget *ManagerWindow::openFile() {
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

		abstractWindow *window = nullptr;
		if (doc.IsNull()) {
			throw std::runtime_error("The file is not in JSON format");
		} else if (doc.IsObject() && Util::rapidjson::getString(doc, "type") == "diagram") {
			window = new diagramWindow(doc);
		} else if (doc.IsObject() && Util::rapidjson::getString(doc, "type") == "link") {
			window = new knotWindow(doc);
		} else {
			throw std::runtime_error("The file does not represent a knot nor a diagram");
		}

		window->show();
		return window;
	} catch (const std::runtime_error &e) {
		abstractWindow::AWRegister.pop_back();
		QMessageBox::critical(0, "File opening error", QString("\n") + e.what() + "\n");
		return nullptr;
	}
}

}}

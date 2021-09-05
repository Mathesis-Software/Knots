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

#include <QtCore/QSettings>
#include <QtWidgets/QMenuBar>

#include "AboutWindow.h"
#include "BaseWindow.h"
#include "KnotEditorApplication.h"
#include "Window.h"

namespace KE::Qt {

BaseWindow::BaseWindow() {
	this->setAttribute(::Qt::WA_DeleteOnClose);
	this->menuBar()->setContextMenuPolicy(::Qt::PreventContextMenu);
}

void BaseWindow::restoreParameters() {
	const QString id = this->identifier();
	if (!id.isNull()) {
		QSettings settings;
		settings.beginGroup("Window:" + id);
		auto geometry = settings.value("geometry").toRect();
		settings.endGroup();
		if (geometry.isValid()) {
			this->setGeometry(geometry);
		}
	}
}

void BaseWindow::createFileMenu() {
	Window *window = dynamic_cast<Window*>(this);

	QMenu *fileMenu = this->menuBar()->addMenu("File");

	auto library = fileMenu->addAction("Library", [] { KnotEditorApplication::library(); });
	library->setShortcut(QKeySequence("Ctrl+L"));
	auto newd = fileMenu->addAction("New diagram", [] { KnotEditorApplication::newDiagram(); });
	newd->setShortcut(QKeySequence("Ctrl+N"));
	auto open = fileMenu->addAction("Open…", [] { KnotEditorApplication::openFile(); });
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
	fileMenu->addAction("About", [] { AboutWindow::showAboutDialog(); });
	fileMenu->addSeparator();
	auto close = fileMenu->addAction("Close", [this] { this->close(); });
	close->setShortcut(QKeySequence("Ctrl+W"));
	auto quit = fileMenu->addAction("Quit", [] {
		dynamic_cast<KnotEditorApplication*>(qApp)->exitApplication();
	});
	quit->setShortcut(QKeySequence("Ctrl+Q"));
}

void BaseWindow::closeEvent(QCloseEvent*) {
	const QString id = this->identifier();
	if (!id.isNull()) {
		QSettings settings;
		settings.beginGroup("Window:" + id);
		settings.setValue("geometry", this->geometry());
		settings.endGroup();
		settings.sync();
	}
}

}

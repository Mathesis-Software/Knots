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

#include <QtGui/QCloseEvent>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>

#include "Window.h"
#include "../manager/about.h"
#include "../manager/iconProvider.h"

namespace KE { namespace Qt {

Window::Window() {
	this->setAttribute(::Qt::WA_DeleteOnClose);

	QMenu *fileMenu = this->menuBar()->addMenu("File");

	auto newd = fileMenu->addAction("New diagram", [this] { Window::newDiagram(); });
	newd->setShortcut(QKeySequence("Ctrl+N"));
	auto open = fileMenu->addAction("Open…", [this] { Window::openFile(); });
	open->setShortcut(QKeySequence("Ctrl+O"));
	fileMenu->addSeparator();
	auto save = fileMenu->addAction("Save as…", [this] { this->save(); });
	save->setShortcut(QKeySequence("Ctrl+S"));
	fileMenu->addAction("Print…", [this] { this->print(); })->setEnabled(false);
	fileMenu->addSeparator();
	fileMenu->addAction("Rename…", [this] { this->rename(); });
	fileMenu->addSeparator();
	fileMenu->addAction("About", [] { Qt::AboutWindow::showAboutDialog(); });
	fileMenu->addSeparator();
	auto close = fileMenu->addAction("Close", [this] { this->close(); });
	close->setShortcut(QKeySequence("Ctrl+W"));
	auto quit = fileMenu->addAction("Quit", [] { Window::exitApplication(); });
	quit->setShortcut(QKeySequence("Ctrl+Q"));

	this->toolbar = new QToolBar(this);
	addToolBar(this->toolbar);

	AWRegister.push_back(this);
}

Window::~Window() {
	delete this->toolbar;
}

int Window::askForSave() {
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

void Window::closeEvent(QCloseEvent *event) {
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
	dialog.setIconProvider(Qt::FileIconProvider::instance());
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

void Window::save() {
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

void Window::print() {
	QPrinter prn;
	//if (prn.setup(this)) {
	//	printIt(&prn);
	//}
}

QAction *Window::addToolbarAction(const QString &iconFilename, const QString &text, const std::function<void()> &functor) {
	const QIcon icon(":images/" + iconFilename);
	return this->toolbar->addAction(icon, text, functor);
}

void Window::addToolbarSeparator() {
	this->toolbar->addSeparator();
}

void Window::complete() {
	this->toolbar->show();
	statusBar()->setVisible(true);
	this->resize(508, 594);
}

QAction *Window::registerAction(QAction *action, std::function<void(QAction&)> controller) {
	this->actionsMap[action] = controller;
	return action;
}

void Window::updateActions() {
	for (auto &[action, controller] : this->actionsMap) {
		controller(*action);
	}
}

}}
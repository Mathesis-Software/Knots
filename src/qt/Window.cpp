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

#include <QtGui/QCloseEvent>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>

#include "FileIconProvider.h"
#include "Window.h"

namespace KE::Qt {

Window::Window() {
	this->toolbar = new QToolBar(this);
	this->toolbar->setMovable(false);
	this->toolbar->setContextMenuPolicy(::Qt::PreventContextMenu);
	addToolBar(this->toolbar);

	this->createFileMenu();
}

bool Window::saveBeforeClosing() {
	this->show();
	this->raise();

	const QString question = "\nSave \"" + this->windowTitle() + "\" before closing?\n";
	while (!this->isSaved()) {
		const int answer = QMessageBox::warning(
			this, "Closing window", question, QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel
		);
		switch (answer) {
			case QMessageBox::Yes:
				this->save();
				break;
			case QMessageBox::No:
				return true;
			default:
				return false;
		}
	}
	return true;
}

void Window::closeEvent(QCloseEvent *event) {
	if (!this->isSaved() && !this->saveBeforeClosing()) {
		event->ignore();
		return;
	}

	emit closing();
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
		QMessageBox::critical(this, "Error", "\nCannot open file \"" + filename + "\" for writing\n");
		return;
	}

	saveIt(os);
	os.close();
}

void Window::exportPNG() {
	auto image = this->exportImage();
	if (image.isNull()) {
		QMessageBox::critical(this, "Error", "\nCannot grab image\n");
		return;
	}

	QString filename = QFileDialog::getSaveFileName(nullptr, "Export as image", QString(), "Image files (*.png *.jpg *.jpeg)");
	if (!filename.isEmpty()) {
		image.save(filename);
	}
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
	QObject::connect(this, &Window::contentChanged, [action, controller] { controller(*action); });
	return action;
}

}

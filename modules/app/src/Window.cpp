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

#include <QtCore/QSettings>
#include <QtCore/QStandardPaths>
#include <QtGui/QCloseEvent>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>

#include "FileIconProvider.h"
#include "Window.h"

namespace KE::Qt {

Window::Window(const QString &filename) : _filename(filename) {
	this->toolbar = new QToolBar(this);
	this->toolbar->setMovable(false);
	this->toolbar->setContextMenuPolicy(::Qt::PreventContextMenu);
	addToolBar(this->toolbar);

	this->createFileMenu();

	this->toolbar->show();
	statusBar()->setVisible(true);
	this->resize(508, 594);

	this->restoreParameters();
}

bool Window::saveBeforeClosing() {
	this->show();
	this->raise();

	const QString question = "\nSave \"" + this->windowTitle() + "\" before closing?\n";
	while (!this->isSaved()) {
		const int answer = QMessageBox::question(
						this, "Closing window", question, QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		switch (answer) {
			case QMessageBox::Save:
				this->save();
				break;
			case QMessageBox::Discard:
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

	BaseWindow::closeEvent(event);
	emit closing();
}

namespace {

QString getSaveFileNameEx(const QString &fileFilter) {
	QSettings settings;
	QString dir = settings.value("CustomFilesFolder").toString();
	if (dir.isEmpty()) {
		dir = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
	}
	QFileDialog dialog(nullptr, "Save file", dir);
	dialog.setSupportedSchemes(QStringList(QStringLiteral("file")));
	dialog.setAcceptMode(QFileDialog::AcceptSave);
	dialog.setIconProvider(Qt::FileIconProvider::instance());
	dialog.setNameFilters({fileFilter,
												 "Any files (*)"});
	if (dialog.exec() == QDialog::Accepted) {
		settings.setValue("CustomFilesFolder", dialog.directory().path());
		settings.sync();
		return dialog.selectedUrls().value(0).toLocalFile();
	}
	return QString();
}

}

void Window::save() {
	const QString filename = getSaveFileNameEx(this->fileFilter());
	if (filename.isEmpty()) {
		return;
	}

	std::ofstream os(filename.toStdString());
	if (!os) {
		QMessageBox::warning(this, "Error", "\nCannot open file \"" + filename + "\" for writing\n");
		return;
	}

	saveIt(os);
	os.close();
	this->_filename = filename;
}

void Window::exportPNG() {
	auto image = this->exportImage();
	if (image.isNull()) {
		QMessageBox::warning(this, "Error", "\nCannot grab image\n");
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

QAction *Window::registerAction(QAction *action, std::function<void(QAction &)> controller) {
	QObject::connect(this, &Window::contentChanged, [action, controller] { controller(*action); });
	return action;
}

QString Window::identifier() const {
	if (this->_filename.isNull()) {
		return QString();
	}
	return QFileInfo(this->_filename).canonicalFilePath();
}

}

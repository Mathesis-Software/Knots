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

#include <QtGui/QCloseEvent>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>

#include "AboutWindow.h"
#include "FileIconProvider.h"
#include "Window.h"

namespace KE::Qt {

Window::Window() {
	this->setAttribute(::Qt::WA_DeleteOnClose);

	QMenu *fileMenu = this->menuBar()->addMenu("File");

	auto newd = fileMenu->addAction("New diagram", [] { Window::newDiagram(); });
	newd->setShortcut(QKeySequence("Ctrl+N"));
	auto open = fileMenu->addAction("Open…", [] { Window::openFile(); });
	open->setShortcut(QKeySequence("Ctrl+O"));
	fileMenu->addSeparator();
	auto save = fileMenu->addAction("Save as…", [this] { this->save(); });
	save->setShortcut(QKeySequence("Ctrl+S"));
	fileMenu->addAction("Export as image…", this, &Window::exportPNG);
	fileMenu->addSeparator();
	fileMenu->addAction("Rename…", [this] { this->rename(); });
	fileMenu->addSeparator();
	fileMenu->addAction("About", [] { Qt::AboutWindow::showAboutDialog(); });
	fileMenu->addSeparator();
	auto close = fileMenu->addAction("Close", [this] { this->close(); });
	close->setShortcut(QKeySequence("Ctrl+W"));
	auto quit = fileMenu->addAction("Quit", [] { Window::exitApplication(); });
	quit->setShortcut(QKeySequence("Ctrl+Q"));

	this->menuBar()->setContextMenuPolicy(::Qt::PreventContextMenu);
	this->toolbar = new QToolBar(this);
	this->toolbar->setMovable(false);
	this->toolbar->setContextMenuPolicy(::Qt::PreventContextMenu);
	addToolBar(this->toolbar);
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

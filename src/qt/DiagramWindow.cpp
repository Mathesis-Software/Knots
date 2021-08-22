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

#include <QtGui/QPainter>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QStatusBar>

#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>

#include "DiagramWindow.h"
#include "KnotWindow.h"

namespace KE::Qt {

DiagramWindow::DiagramWindow(const rapidjson::Document &doc) {
	this->init(new DiagramWidget(this, doc));
}

DiagramWindow::DiagramWindow() {
	this->init(new DiagramWidget(this));
}

void DiagramWindow::init(DiagramWidget *widget) {
	setCentralWidget(widget);
	const auto &diagram = widget->diagram;

	this->connect(widget, &DiagramWidget::setActionTip, [this](const QString &text) {
		if (!text.isNull()) {
			this->statusBar()->showMessage(text);
		} else {
			this->statusBar()->clearMessage();
		}
	});
	this->connect(widget, &DiagramWidget::actionsUpdated, this, &DiagramWindow::updateActions);

	actionsMenu = this->menuBar()->addMenu("Actions");
	this->registerAction(
		actionsMenu->addAction("Convert", [this] { this->convert(); }),
		[&diagram](QAction &action) { action.setEnabled(diagram.isClosed()); }
	);
	this->registerAction(
		actionsMenu->addAction("Simplify", [this] { this->simplify(); }),
		[&diagram](QAction &action) { action.setEnabled(diagram.isClosed()); }
	);
	this->registerAction(
		actionsMenu->addAction("Clear", [this] { this->clear(); }),
		[&diagram](QAction &action) { action.setEnabled(!diagram.vertices().empty()); }
	);

	this->registerAction(
		this->addToolbarAction("trefoil.png", "Convert to knot", [this] { this->convert(); }),
		[&diagram](QAction &action) {
			const bool enabled = diagram.isClosed();
			action.setEnabled(enabled);
			action.setToolTip(enabled ? "Converting to 3D knot" : "Converting to knot disabled until the diagram is closed.");
		}
	);

	addToolbarSeparator();
	auto addAction = [this, widget](const QString &icon, const QString &text, const QString &tooltip, const QString &disabledTooltip, DiagramWidget::EditorMode mode) {
		QAction *action = this->addToolbarAction(icon, text, [this, mode] { this->setMode(mode); });
		action->setCheckable(true);
		this->registerAction(action, [widget, mode, tooltip, disabledTooltip](QAction &action) {
			const bool canSet = widget->canSetEditorMode(mode);
			action.setChecked(canSet && widget->editorMode() == mode);
			action.setEnabled(canSet);
			action.setToolTip(canSet ? tooltip : disabledTooltip);
		});
	};
	addAction(
		"diagram_mode_quick_drawing.svg",
		"Quick drawing",
		"Quick drawing: move the mouse and click to set a new point. Right-click to close the diagram.",
		"Quick drawing disabled for closed diagram.",
		DiagramWidget::QUICK_DRAWING
	);
	addAction(
		"diagram_mode_editing.svg",
		"Editing",
		"Editing: choose point/edge/crossing and click to change it. Read status at the bottom of the window if unsure.",
		"Editing disabled for empty diagram.",
		DiagramWidget::EDITING
	);
	addAction(
		"diagram_mode_moving.svg",
		"Moving diagram",
		"Moving: click anywhere and move the diagram.",
		"Moving disabled for empty diagram.",
		DiagramWidget::MOVING
	);

	addToolbarSeparator();
	auto undo = this->registerAction(
		this->addToolbarAction("undo.svg", "Undo", [this, widget] {
			widget->diagram.undo();
			widget->repaint();
			widget->updateEditorMode();
			this->updateActions();
		}),
		[widget](QAction &action) { action.setEnabled(widget->diagram.canUndo()); }
	);
	undo->setShortcut(QKeySequence("Ctrl+Z"));
	auto redo = this->registerAction(
		this->addToolbarAction("redo.svg", "Redo", [this, widget] {
			widget->diagram.redo();
			widget->repaint();
			widget->updateEditorMode();
			this->updateActions();
		}),
		[widget](QAction &action) { action.setEnabled(widget->diagram.canRedo()); }
	);
	redo->setShortcut(QKeySequence("Ctrl+Shift+Z"));

	QPixmap pixmap(":images/diagram.svg");
	pixmap.setDevicePixelRatio(this->devicePixelRatio());
	this->setWindowIcon(pixmap);

	complete();

	QObject::connect(this, &Window::contentChanged, [this] {
		this->setWindowTitle(this->diagramWidget()->diagram.caption().c_str());
	});

	this->updateActions();
}

DiagramWindow::~DiagramWindow() {
	delete actionsMenu;
}

void DiagramWindow::setMode(DiagramWidget::EditorMode mode) {
	this->diagramWidget()->setEditorMode(mode);
	this->updateActions();
}

void DiagramWindow::clear() {
	this->diagramWidget()->clear();
	this->updateActions();
}

void DiagramWindow::convert() {
	if (!this->diagramWidget()->diagram.isClosed()) {
		QMessageBox::critical(this, "Error", "\nCannot convert non-closed diagram.\n");
		return;
	}

	if (this->diagramWidget()->diagram.vertices().size() <= 2) {
		QMessageBox::critical(this, "Error", "\nCannot convert diagram with less than three points.\n");
		return;
	}

	(new KnotWindow(*this->diagramWidget()))->show();
}

void DiagramWindow::saveIt(std::ostream &os) {
	const rapidjson::Document doc = this->diagramWidget()->diagram.serialize();
	rapidjson::OStreamWrapper wrapper(os);
	rapidjson::Writer<rapidjson::OStreamWrapper> writer(wrapper);
	doc.Accept(writer);
}

void DiagramWindow::simplify() {
	if (this->diagramWidget()->diagram.simplify()) {
		this->centralWidget()->repaint();
	}
}

QImage DiagramWindow::exportImage() const {
	const auto widget = this->diagramWidget();
	const auto size = widget->size();
	const auto dpr = widget->devicePixelRatio();
	QImage image(size.width() * dpr, size.height() * dpr, QImage::Format_RGB32);
	QPainter painter;
	image.setDevicePixelRatio(dpr);
	image.fill(::Qt::white);
	painter.begin(&image);
	widget->drawIt(painter);
	painter.end();
	return image;
}

void DiagramWindow::rename() {
	auto &diagram = this->diagramWidget()->diagram;

	bool ok;
	const QString text = QInputDialog::getText(
		this, "Rename diagram", "New diagram name:", QLineEdit::Normal, diagram.caption().c_str(), &ok
	);
	if (ok) {
		diagram.setCaption(text.toStdString());
		this->updateActions();
	}
}

bool DiagramWindow::isSaved() const {
	const auto widget = this->diagramWidget();
	return !widget || widget->diagram.isSaved();
}

}

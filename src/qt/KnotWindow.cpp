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
#include <QtGui/QPainter>
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QStatusBar>

#include "DiagramWidget.h"
#include "KnotWidget.h"
#include "KnotWindow.h"

namespace KE::Qt {

KnotWindow::KnotWindow(const rapidjson::Document &doc) {
  this->init(new KnotWidget(this, doc));
}

KnotWindow::KnotWindow(const DiagramWidget &diagramWidget) {
  this->init(new KnotWidget(this, diagramWidget.diagram.diagram(), diagramWidget.width(), diagramWidget.height()));
}

void KnotWindow::init(KnotWidget *widget) {
	this->setCentralWidget(widget);

	QObject::connect(widget, &KnotWidget::setActionTip, [this](const QString &text) {
		if (!text.isNull()) {
			this->statusBar()->showMessage(text);
		} else {
			this->statusBar()->clearMessage();
		}
	});
	QObject::connect(widget, &KnotWidget::actionsUpdated, [this] { emit this->contentChanged(); });
	QObject::connect(this, &Window::contentChanged, [this] {
  	this->setWindowTitle(this->knotWidget()->knot().caption().c_str());
	});

  initMenu();
  complete();

	emit this->contentChanged();
}

void KnotWindow::initMenu() {
  knotMenu = menuBar()->addMenu("Knot");
  knotMenu->addAction("Computables…", [this] { this->showMathDialog(); });
  knotMenu->addSeparator();
	this->registerAction(
		knotMenu->addAction("Start smoothing", [this] { this->knotWidget()->startSmoothing(); }),
		[this](QAction &action) { action.setEnabled(!this->knotWidget()->isSmoothingInProgress()); }
	);
	this->registerAction(
		knotMenu->addAction("Stop smoothing", [this] { this->knotWidget()->stopSmoothing(); }),
		[this](QAction &action) { action.setEnabled(this->knotWidget()->isSmoothingInProgress()); }
	);
  knotMenu->addSeparator();
  knotMenu->addAction("Visual options…", [this] { this->showOptionsDialog(); });
  knotMenu->addAction("Number of points…", [this] { this->knotWidget()->setNumberOfPoints(); });
  knotMenu->addAction("Length…", [this] { this->knotWidget()->setLength(); });
  
	this->registerAction(
		addToolbarAction("magic_start.svg", "Start smoothing", [this] { this->knotWidget()->startSmoothing(); }),
		[this](QAction &action) { action.setVisible(!this->knotWidget()->isSmoothingInProgress()); }
	);
	this->registerAction(
		addToolbarAction("magic_stop.svg", "Interrupt smoothing", [this] { this->knotWidget()->stopSmoothing(); }),
		[this](QAction &action) { action.setVisible(this->knotWidget()->isSmoothingInProgress()); }
	);
  addToolbarSeparator();
  addToolbarAction("math.svg", "Show parameters", [this] { this->showMathDialog(); });
  addToolbarSeparator();
  this->registerAction(
		addToolbarAction("plus.svg", "Shift Seifert surface forward along the gradient", [this] { this->knotWidget()->moveSeifertBasePoint(0.02); }),
		[this](QAction &action) { action.setEnabled(this->knotWidget()->isSeifertSurfaceVisible()); }
	);
  this->registerAction(
		addToolbarAction("minus.svg", "Shift Seifert surface back along the gradient", [this] { this->knotWidget()->moveSeifertBasePoint(-0.02); }),
		[this](QAction &action) { action.setEnabled(this->knotWidget()->isSeifertSurfaceVisible()); }
	);
}

KnotWindow::~KnotWindow() {
  delete knotMenu;
}

void KnotWindow::closeEvent(QCloseEvent *event) {
	Window::closeEvent(event);
	if (event->isAccepted()) {
		this->knotWidget()->stopSmoothingAndWait();
	}
}

void KnotWindow::rename() {
	bool ok;
	const QString text = QInputDialog::getText(
		this, "Rename knot", "New knot name:", QLineEdit::Normal, this->knotWidget()->knot().caption().c_str(), &ok
	);
  if (ok) {
		this->knotWidget()->setCaption(text);
		emit this->contentChanged();
	}
}

KnotWidget *KnotWindow::knotWidget() const {
	return dynamic_cast<KnotWidget*>(this->centralWidget());
}

bool KnotWindow::isSaved() const {
	auto widget = this->knotWidget();
	return !widget || widget->knot().isSaved();
}

void KnotWindow::saveIt(std::ostream &os) {
	this->knotWidget()->saveKnot(os);
}

QImage KnotWindow::exportImage() const {
	return this->knotWidget()->grabFramebuffer();
}

}

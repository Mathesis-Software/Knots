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
#include <QtWidgets/QInputDialog>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QStatusBar>

#include "knotWindow.h"
#include "knotWindow_math.h"
#include "KnotWidget.h"
#include "../diagram/DiagramWidget.h"

void knotWindow::init(KnotWidget *widget) {
	this->setCentralWidget(widget);

	this->connect(widget, &KnotWidget::setActionTip, [this](const QString &text) {
		if (!text.isNull()) {
			this->statusBar()->showMessage(text);
		} else {
			this->statusBar()->clearMessage();
		}
	});
	this->connect(widget, &KnotWidget::actionsUpdated, this, &knotWindow::updateActions);

  mth = NULL;

  initMenu();
  complete();

	this->updateActions();
}

knotWindow::knotWindow(const rapidjson::Document &doc) {
  this->init(new KnotWidget(this, doc));
}

knotWindow::knotWindow(const DiagramWidget &diagramWidget) {
  this->init(new KnotWidget(this, diagramWidget.diagram.diagram(), diagramWidget.width(), diagramWidget.height()));
}

knotWindow::~knotWindow() {
  if (mth) {
    delete mth;
	}

  delete mathMenu;
  delete viewMenu;
}

void knotWindow::closeEvent(QCloseEvent *event) {
	abstractWindow::closeEvent(event);
	if (event->isAccepted()) {
		this->knotWidget()->stopSmoothingAndWait();
	}
}

void knotWindow::updateActions() {
  setWindowTitle(this->knotWidget()->knot().caption().c_str());
	if (mth) {
		mth->recompute();
	}
	abstractWindow::updateActions();
}

void knotWindow::rename() {
	bool ok;
	const QString text = QInputDialog::getText(
		this, "Rename knot", "New knot name:", QLineEdit::Normal, this->knotWidget()->knot().caption().c_str(), &ok
	);
  if (ok) {
		this->knotWidget()->setCaption(text);
		this->updateActions();
	}
}

KnotWidget *knotWindow::knotWidget() const {
	return dynamic_cast<KnotWidget*>(this->centralWidget());
}

bool knotWindow::isSaved() const {
	auto widget = this->knotWidget();
	return !widget || widget->isKnotSaved();
}

void knotWindow::saveIt(std::ostream &os) {
	this->knotWidget()->saveKnot(os);
}

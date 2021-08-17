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

#include "KnotWindow.h"
#include "KnotWindow_math.h"
#include "../widget/KnotWidget.h"
#include "../widget/DiagramWidget.h"

namespace KE { namespace Qt {

void KnotWindow::init(KnotWidget *widget) {
	this->setCentralWidget(widget);

	this->connect(widget, &KnotWidget::setActionTip, [this](const QString &text) {
		if (!text.isNull()) {
			this->statusBar()->showMessage(text);
		} else {
			this->statusBar()->clearMessage();
		}
	});
	this->connect(widget, &KnotWidget::actionsUpdated, this, &KnotWindow::updateActions);

  mth = NULL;

  initMenu();
  complete();

	this->updateActions();
}

KnotWindow::KnotWindow(const rapidjson::Document &doc) {
  this->init(new KnotWidget(this, doc));
}

KnotWindow::KnotWindow(const DiagramWidget &diagramWidget) {
  this->init(new KnotWidget(this, diagramWidget.diagram.diagram(), diagramWidget.width(), diagramWidget.height()));
}

KnotWindow::~KnotWindow() {
  if (mth) {
    delete mth;
	}

  delete mathMenu;
  delete viewMenu;
}

void KnotWindow::closeEvent(QCloseEvent *event) {
	Window::closeEvent(event);
	if (event->isAccepted()) {
		this->knotWidget()->stopSmoothingAndWait();
	}
}

void KnotWindow::updateActions() {
  setWindowTitle(this->knotWidget()->knot().caption().c_str());
	if (mth) {
		mth->recompute();
	}
	Window::updateActions();
}

void KnotWindow::rename() {
	bool ok;
	const QString text = QInputDialog::getText(
		this, "Rename knot", "New knot name:", QLineEdit::Normal, this->knotWidget()->knot().caption().c_str(), &ok
	);
  if (ok) {
		this->knotWidget()->setCaption(text);
		this->updateActions();
	}
}

KnotWidget *KnotWindow::knotWidget() const {
	return dynamic_cast<KnotWidget*>(this->centralWidget());
}

bool KnotWindow::isSaved() const {
	auto widget = this->knotWidget();
	return !widget || widget->isKnotSaved();
}

void KnotWindow::saveIt(std::ostream &os) {
	this->knotWidget()->saveKnot(os);
}

void KnotWindow::printIt(QPrinter *prn) {
	QPainter pnt;
	pnt.begin(prn);
	raise();
	pnt.drawPixmap(0, 0, QPixmap::grabWindow(centralWidget()->winId()));
	pnt.end();
}

}}

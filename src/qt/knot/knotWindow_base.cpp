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

#include "knotWindow.h"
#include "knotWindow_math.h"
#include "KnotWidget.h"
#include "../diagram/diagramWindow.h"
#include "../../math/knotSurface/KnotSurface.h"
#include "../../math/seifert/SeifertSurface.h"

void knotWindow::init() {
	this->setCentralWidget(new KnotWidget(this, this->knot));
  this->knotSurface = std::make_shared<KE::GL::KnotSurface>(this->knot, 28);
  this->knotWidget()->addSurface(this->knotSurface);
  this->seifertSurface = std::make_shared<KE::GL::SeifertSurface>(this->knot, this->seifertStartPoint);
  this->knotWidget()->addSurface(this->seifertSurface);

  mth = NULL;

  initMenu();
  complete();

	this->updateActions();
}

knotWindow::knotWindow(const rapidjson::Document &doc) : knot(doc), seifertStartPoint(0.0, 0.0, 0.4), smoothingThread(*this) {
  this->init();
}

knotWindow::knotWindow(const diagramWindow &d) : knot(d.diagramWidget()->diagram.diagram(), d.width(), d.height()), seifertStartPoint(0.0, 0.0, 0.4), smoothingThread(*this) {
  this->init();
}

knotWindow::~knotWindow() {
  if (mth) {
    delete mth;
	}

  delete mathMenu;
  delete viewMenu;
}

void knotWindow::toggleSeifertSurfaceVisibility() {
  if (this->seifertSurface->isVisible()) {
    this->knot.isSeifertSurfaceVisible = std::make_shared<bool>(false);
  } else {
    this->knot.isSeifertSurfaceVisible = std::make_shared<bool>(true);
  }
	this->seifertSurface->destroy(true);

  this->centralWidget()->update();
	this->updateActions();
}

void knotWindow::closeEvent(QCloseEvent *event) {
	abstractWindow::closeEvent(event);
	if (event->isAccepted() && this->smoothingThread.isRunning()) {
		this->smoothingThread.requestInterruption();
		this->smoothingThread.wait();
	}
}

void knotWindow::updateActions() {
  setWindowTitle(this->knot.caption().c_str());
	abstractWindow::updateActions();
}

void knotWindow::rename() {
	bool ok;
	const QString text = QInputDialog::getText(
		this, "Rename knot", "New knot name:", QLineEdit::Normal, this->knot.caption().c_str(), &ok
	);
  if (ok) {
		this->knot.setCaption(text.toStdString());
		this->updateActions();
	}
}

void knotWindow::moveSeifertBasePoint(double distance) {
	this->seifertStartPoint.move(
		KE::GL::SeifertSurface::gradient(this->seifertStartPoint, this->knot.snapshot()), distance
	);
	this->seifertSurface->destroy(true);
	this->centralWidget()->update();
}

KnotWidget *knotWindow::knotWidget() const {
	return dynamic_cast<KnotWidget*>(this->centralWidget());
}

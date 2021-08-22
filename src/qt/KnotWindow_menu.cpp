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

#include <QtWidgets/QMenuBar>

#include "KnotWidget.h"
#include "KnotWindow.h"

namespace KE::Qt {

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

	this->updateActions();
}

}
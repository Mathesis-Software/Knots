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

#include "knotWindow.h"
#include "../widget/KnotWidget.h"

void knotWindow::initMenu() {
  mathMenu = menuBar()->addMenu("Math");
  mathMenu->addAction("View parameters", [this] { this->math(); });
  mathMenu->addSeparator();
	this->registerAction(
		mathMenu->addAction("Start smoothing", [this] { this->knotWidget()->startSmoothing(); }),
		[this](QAction &action) { action.setEnabled(!this->knotWidget()->isSmoothingInProgress()); }
	);
	this->registerAction(
		mathMenu->addAction("Stop smoothing", [this] { this->knotWidget()->stopSmoothing(); }),
		[this](QAction &action) { action.setEnabled(this->knotWidget()->isSmoothingInProgress()); }
	);
  mathMenu->addSeparator();
  mathMenu->addAction("Number of points…", [this] { this->knotWidget()->setNumberOfPoints(); });
  mathMenu->addAction("Length…", [this] { this->knotWidget()->setLength(); });
  
  viewMenu = menuBar()->addMenu("View");
	auto toggleSeifert = this->registerAction(
		viewMenu->addAction("Show Seifert surface", [this] { this->knotWidget()->toggleSeifertSurfaceVisibility(); }),
		[this](QAction &action) {
			action.setChecked(this->knotWidget()->isSeifertSurfaceVisible());  
		}
	);
	toggleSeifert->setCheckable(true);
  
  QMenu *optionsMenu = menuBar()->addMenu("Options");
  optionsMenu->addAction("Thickness…", [this] { this->knotWidget()->setThickness(); });
  optionsMenu->addSeparator();
  optionsMenu->addAction("Background color…", [this] { this->knotWidget()->setBgColor(); });
  optionsMenu->addAction("Knot color…", [this] { this->knotWidget()->setKnotColor(); });
  optionsMenu->addAction("Seifert surface front color…", [this] { this->knotWidget()->setSeifertFrontColor(); });
  optionsMenu->addAction("Seifert surface back color…", [this] { this->knotWidget()->setSeifertBackColor(); });

	this->registerAction(
		addToolbarAction("smooth.svg", "Start smoothing", [this] { this->knotWidget()->startSmoothing(); }),
		[this](QAction &action) { action.setVisible(!this->knotWidget()->isSmoothingInProgress()); }
	);
	this->registerAction(
		addToolbarAction("stop.svg", "Interrupt smoothing", [this] { this->knotWidget()->stopSmoothing(); }),
		[this](QAction &action) { action.setVisible(this->knotWidget()->isSmoothingInProgress()); }
	);
  addToolbarSeparator();
  addToolbarAction("math.svg", "Show parameters", [this] { this->math(); });
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

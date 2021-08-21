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

#include "KnotWidget.h"

#include "../math/knotWrapper/KnotSurface.h"
#include "../math/seifert/SeifertSurface.h"

namespace KE::Qt {

KnotWidget::KnotWidget(QWidget *parent, const rapidjson::Document &doc) : GLWidget(parent), _knot(doc), smoothingThread(this) {
	this->init();
}

KnotWidget::KnotWidget(QWidget *parent, const TwoD::Diagram &diagram, std::size_t width, std::size_t height) : GLWidget(parent), _knot(diagram, width, height), smoothingThread(this) {
	this->init();
}

void KnotWidget::init() {
  this->addSurface(this->knot().knotSurface());
  this->addSurface(this->knot().seifertSurface());
}

const Color &KnotWidget::backgroundColor() const {
	return this->knot().backgroundColor();
}

void KnotWidget::moveSeifertBasePoint(double distance) {
	this->_knot.moveSeifertBasePoint(distance);
	this->update();
}

void KnotWidget::setSeifertSurfaceVisibility(bool visible) {
	this->_knot.setSeifertSurfaceVisibility(visible);
  this->update();
	emit actionsUpdated();
}

void KnotWidget::onKnotChanged(bool force) {
  if (force || this->knot().knotSurface()->isObsolete() || this->knot().seifertSurface()->isObsolete()) {
		this->update();
		emit actionsUpdated();
	}
}

bool KnotWidget::isSeifertSurfaceVisible() const {
	return this->knot().seifertSurface()->isVisible();
}

bool KnotWidget::isSmoothingInProgress() const {
	return this->smoothingThread.isRunning();
}

}

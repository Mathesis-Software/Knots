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

#include "../../math/knotSurface/KnotSurface.h"
#include "../../math/seifert/SeifertSurface.h"

KnotWidget::KnotWidget(QWidget *parent, const rapidjson::Document &doc) : GLWidget(parent), _knot(doc), seifertStartPoint(0.0, 0.0, 0.4), smoothingThread(this) {
	this->init();
}

KnotWidget::KnotWidget(QWidget *parent, const KE::TwoD::Diagram &diagram, std::size_t width, std::size_t height) : GLWidget(parent), _knot(diagram, width, height), seifertStartPoint(0.0, 0.0, 0.4), smoothingThread(this) {
	this->init();
}

void KnotWidget::init() {
  this->knotSurface = std::make_shared<KE::GL::KnotSurface>(this->knot(), 28);
  this->addSurface(this->knotSurface);
  this->seifertSurface = std::make_shared<KE::GL::SeifertSurface>(this->knot(), this->seifertStartPoint);
  this->addSurface(this->seifertSurface);
}

const KE::GL::Color &KnotWidget::backgroundColor() const {
	const auto ref = this->knot().backgroundColor;
	return ref ? *ref : KE::GL::Color::white;
}

void KnotWidget::moveSeifertBasePoint(double distance) {
	this->seifertStartPoint.move(
		KE::GL::SeifertSurface::gradient(this->seifertStartPoint, this->knot().snapshot()), distance
	);
	this->seifertSurface->destroy();
	this->update();
}

void KnotWidget::toggleSeifertSurfaceVisibility() {
  if (this->seifertSurface->isVisible()) {
    this->_knot.isSeifertSurfaceVisible = std::make_shared<bool>(false);
  } else {
    this->_knot.isSeifertSurfaceVisible = std::make_shared<bool>(true);
  }

  this->update();
	emit actionsUpdated();
}

void KnotWidget::onKnotChanged(bool force) {
  if (force || this->knotSurface->isObsolete() || this->seifertSurface->isObsolete()) {
		this->update();
		emit actionsUpdated();
	}
}

bool KnotWidget::isSeifertSurfaceVisible() const {
	return this->seifertSurface->isVisible();
}

bool KnotWidget::isSmoothingInProgress() const {
	return this->smoothingThread.isRunning();
}

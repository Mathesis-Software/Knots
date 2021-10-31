/*
 * Copyright (c) 1995-2021, Mathesis Software <mad@mathesis.fun>
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

#include <iostream>

#include <QtWidgets/QInputDialog>

#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>

#include <KnotSurface.h>
#include <SeifertSurface.h>

#include "KnotWidget.h"

namespace KE::Qt {

KnotWidget::KnotWidget(QWidget *parent, const rapidjson::Value &doc) : GLWidget(parent), knot(doc), smoothingThread(this) {
	this->init();
}

KnotWidget::KnotWidget(QWidget *parent, const TwoD::Diagram &diagram, std::size_t width, std::size_t height) : GLWidget(parent), knot(diagram, width, height), smoothingThread(this) {
	this->init();
}

void KnotWidget::init() {
	this->addSurface(this->knot.knotSurface());
	this->addSurface(this->knot.seifertSurface());
}

const Color &KnotWidget::backgroundColor() const {
	return this->knot.backgroundColor();
}

void KnotWidget::moveSeifertBasePoint(double distance) {
	this->knot.moveSeifertBasePoint(distance);
	this->update();
}

void KnotWidget::setSeifertSurfaceVisibility(bool visible) {
	this->knot.setSeifertSurfaceVisibility(visible);
	this->update();
	emit actionsUpdated();
}

void KnotWidget::onKnotChanged(bool force) {
	if (force || this->knot.knotSurface()->isObsolete() || this->knot.seifertSurface()->isObsolete()) {
		this->update();
		emit actionsUpdated();
	}
}

bool KnotWidget::isSmoothingInProgress() const {
	return this->smoothingThread.isRunning();
}

void KnotWidget::prepareMatrix(double *matrix, bool inverse) const {
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			if (inverse) {
				matrix[4 * i + j] = this->knot.rotationMatrixElement(j, i);
			} else {
				matrix[4 * i + j] = this->knot.rotationMatrixElement(i, j);
			}
		}
	}
}

void KnotWidget::rotate(double dx, double dy, double dz) {
	this->knot.rotate(dx, dy, dz);
}

void KnotWidget::saveKnot(std::ostream &os) {
	const rapidjson::Document doc = this->knot.serialize();
	rapidjson::OStreamWrapper wrapper(os);
	rapidjson::Writer<rapidjson::OStreamWrapper> writer(wrapper);
	writer.SetMaxDecimalPlaces(5);
	doc.Accept(writer);
	this->knot.setSaveCheckpoint(doc);
}

void KnotWidget::setLength() {
	const auto snapshot = this->knot.snapshot();
	const double d = QInputDialog::getDouble(nullptr, "Set value", "Knot length", snapshot.knotLength(), 1.0, 1000.0, 4);
	if (d != snapshot.knotLength()) {
		this->knot.setLength(d);
		this->knot.center();
		this->update();
		emit actionsUpdated();
	}
}

void KnotWidget::setNumberOfPoints() {
	const std::size_t numberOfPoints = QInputDialog::getInt(nullptr, "Set value", "Number of points", this->knot.snapshot().size(), 10, 30000, 10);
	if (numberOfPoints != this->knot.snapshot().size()) {
		const double length = this->knot.snapshot().knotLength();
		this->knot.normalize(numberOfPoints);
		this->knot.center();
		this->knot.setLength(length);
		this->update();
		emit actionsUpdated();
	}
}

}// namespace KE::Qt

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

#include <QtWidgets/QColorDialog>

#include "KnotWidget.h"
#include "../setValue/setValue.h"
#include "../../math/knotSurface/KnotSurface.h"
#include "../../math/seifert/SeifertSurface.h"

namespace KE { namespace Qt {

void KnotWidget::setLength() {
	const auto snapshot = this->knot().snapshot();
  double d = setDouble ("Knot length", snapshot.knotLength(), 1.0, 1000.0);
  if (d != snapshot.knotLength()) {
    this->_knot.setLength(d);
    this->_knot.center();
    this->update();
		emit actionsUpdated();
  }
}

void KnotWidget::setNumberOfPoints() {
	const std::size_t numberOfPoints = setInt("Set number of points", this->knot().snapshot().size(), 10, 30000);
  if (numberOfPoints != this->knot().snapshot().size()) {
		const double length = this->knot().snapshot().knotLength();
    this->_knot.normalize(numberOfPoints);
    this->_knot.center();
		this->_knot.setLength(length);
    this->update();
		emit actionsUpdated();
  }
}

void KnotWidget::setThickness() {
  const double thickness = setDouble("Thickness", this->knot().knotSurface->thickness(), 0.1, 10.0);
  if (thickness != this->knot().knotSurface->thickness()) {
		this->_knot.knotThickness = std::make_shared<double>(thickness);
    this->knot().knotSurface->destroy();
		this->update();
  }
}

namespace {

QColor from(const GL::Color &color) {
	return QColor((int)round(color.rgb[0] * 255), (int)round(color.rgb[1] * 255), (int)round(color.rgb[2] * 255));
}

void runColorDialog(const QString &title, std::function<QColor()> getter, std::function<void(const QColor&)> setter) {
	const QColor initial = getter();
	QColorDialog dialog(initial, nullptr);
	dialog.setWindowTitle(title);
	QObject::connect(&dialog, &QColorDialog::currentColorChanged, setter);
	if (dialog.exec() == QColorDialog::Rejected) {
		setter(initial);
	}
}

}

void KnotWidget::setBgColor() {
	runColorDialog(
		"Background Color",
		[this] { return from(this->backgroundColor()); },
		[this] (const QColor &color) {
			this->_knot.backgroundColor = std::make_shared<GL::Color>(color.red(), color.green(), color.blue());
			this->update();
		}
	);
}

void KnotWidget::setKnotColor() {
	runColorDialog(
		"Knot Color",
		[this] { return from(this->knot().knotSurface->frontColor()); },
		[this] (const QColor &color) {
			this->_knot.knotColor = std::make_shared<GL::Color>(color.red(), color.green(), color.blue());
			this->update();
		}
	);
}

void KnotWidget::setSeifertFrontColor() {
	runColorDialog(
		"Seifert Surface Color",
		[this] { return from(this->knot().seifertSurface->frontColor()); },
		[this] (const QColor &color) {
			this->_knot.seifertFrontColor = std::make_shared<GL::Color>(color.red(), color.green(), color.blue());
			this->update();
		}
	);
}

void KnotWidget::setSeifertBackColor() {
	runColorDialog(
		"Seifert Surface Back Side Color",
		[this] { return from(this->knot().seifertSurface->backColor()); },
		[this] (const QColor &color) {
			this->_knot.seifertBackColor = std::make_shared<GL::Color>(color.red(), color.green(), color.blue());
			this->update();
		}
	);
}

}}

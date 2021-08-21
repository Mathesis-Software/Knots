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

#include <QtWidgets/QInputDialog>

#include "KnotWidget.h"

namespace KE { namespace Qt {

void KnotWidget::setLength() {
	const auto snapshot = this->knot().snapshot();
  const double d = QInputDialog::getDouble(nullptr, "Set value", "Knot length", snapshot.knotLength(), 1.0, 1000.0, 4);
  if (d != snapshot.knotLength()) {
    this->_knot.setLength(d);
    this->_knot.center();
    this->update();
		emit actionsUpdated();
  }
}

void KnotWidget::setNumberOfPoints() {
	const std::size_t numberOfPoints = QInputDialog::getInt(nullptr, "Set value", "Number of points", this->knot().snapshot().size(), 10, 30000, 10);
  if (numberOfPoints != this->knot().snapshot().size()) {
		const double length = this->knot().snapshot().knotLength();
    this->_knot.normalize(numberOfPoints);
    this->_knot.center();
		this->_knot.setLength(length);
    this->update();
		emit actionsUpdated();
  }
}

}}

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

#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/writer.h>

#include "KnotWidget.h"

namespace KE::Qt {

bool KnotWidget::isKnotSaved() const {
	double matrix[3][3];
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			matrix[i][j] = this->currMatr(i, j);
		}
	}
	return this->knot().isSaved(matrix);
}

void KnotWidget::saveKnot(std::ostream &os) {
	double matrix[3][3];
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			matrix[i][j] = this->currMatr(i, j);
		}
	}
	const rapidjson::Document doc = this->_knot.serialize(matrix);
	rapidjson::OStreamWrapper wrapper(os);
	rapidjson::Writer<rapidjson::OStreamWrapper> writer(wrapper);
	writer.SetMaxDecimalPlaces(5);
	doc.Accept(writer);
}

}

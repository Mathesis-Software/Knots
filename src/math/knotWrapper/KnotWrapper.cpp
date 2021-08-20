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

#include "KnotWrapper.h"
#include "../knotSurface/KnotSurface.h"
#include "../seifert/SeifertSurface.h"
#include "../util/rapidjson.h"

namespace KE { namespace ThreeD {

KnotWrapper::KnotWrapper(const TwoD::Diagram &diagram, std::size_t width, std::size_t height) : knot(diagram, width, height), seifertStartPoint(0.0, 0.0, 0.4) {
	this->init();
}

KnotWrapper::KnotWrapper(const rapidjson::Document &doc) : knot(doc), seifertStartPoint(0.0, 0.0, 0.4) {
	this->readUiOptions(doc);
	this->init();
}

void KnotWrapper::init() {
	auto cp = this->knot.serialize();
	this->saveUiOptions(cp);
	this->saveCheckpoint = Util::rapidjson::docToString(cp);
  this->knotSurface = std::make_shared<GL::KnotSurface>(*this, 28);
  this->seifertSurface = std::make_shared<GL::SeifertSurface>(*this, this->seifertStartPoint);
}

rapidjson::Document KnotWrapper::serialize(const double matrix[3][3]) {
	auto doc = this->knot.serialize(matrix);
	this->saveUiOptions(doc);
	this->saveCheckpoint = Util::rapidjson::docToString(doc);
	return doc;
}

bool KnotWrapper::isSaved(const double matrix[3][3]) const {
	auto doc = this->knot.serialize(matrix);
	this->saveUiOptions(doc);
	return this->saveCheckpoint == Util::rapidjson::docToString(doc);
}

void KnotWrapper::saveUiOptions(rapidjson::Document &doc) const {
	rapidjson::Value componentUi(rapidjson::kObjectType);
	if (this->knotColor) {
		const std::string sv = this->knotColor->stringValue();
		rapidjson::Value color;
		color.SetString(sv.data(), sv.size(), doc.GetAllocator());
		componentUi.AddMember("color", color, doc.GetAllocator());
	}
	if (this->knotThickness) {
		componentUi.AddMember("thickness", *this->knotThickness, doc.GetAllocator());
	}
	doc["components"][0].AddMember("ui", componentUi, doc.GetAllocator());

	rapidjson::Value globalUi(rapidjson::kObjectType);
	if (this->isSeifertSurfaceVisible) {
		globalUi.AddMember("isSeifertSurfaceVisible", *this->isSeifertSurfaceVisible, doc.GetAllocator());
	}
	if (this->backgroundColor) {
		const std::string sv = this->backgroundColor->stringValue();
		rapidjson::Value color;
		color.SetString(sv.data(), sv.size(), doc.GetAllocator());
		globalUi.AddMember("backgroundColor", color, doc.GetAllocator());
	}
	if (this->seifertFrontColor) {
		const std::string sv = this->seifertFrontColor->stringValue();
		rapidjson::Value color;
		color.SetString(sv.data(), sv.size(), doc.GetAllocator());
		globalUi.AddMember("seifertFrontColor", color, doc.GetAllocator());
	}
	if (this->seifertBackColor) {
		const std::string sv = this->seifertBackColor->stringValue();
		rapidjson::Value color;
		color.SetString(sv.data(), sv.size(), doc.GetAllocator());
		globalUi.AddMember("seifertBackColor", color, doc.GetAllocator());
	}
	doc.AddMember("ui", globalUi, doc.GetAllocator());
}

void KnotWrapper::readUiOptions(const rapidjson::Document &doc) {
	const auto &component = doc["components"][0];
	if (component.HasMember("ui") && component["ui"].IsObject()) {
		const auto &ui = component["ui"];
		this->knotColor = GL::Color::parse(Util::rapidjson::getString(ui, "color"));
		if (ui.HasMember("thickness") && ui["thickness"].IsNumber()) {
			this->knotThickness = std::make_shared<double>(ui["thickness"].GetDouble());
		}
	}

	if (doc.HasMember("ui") && doc["ui"].IsObject()) {
		const auto &ui = doc["ui"];
		this->backgroundColor = GL::Color::parse(Util::rapidjson::getString(ui, "backgroundColor"));
		this->seifertFrontColor = GL::Color::parse(Util::rapidjson::getString(ui, "seifertFrontColor"));
		this->seifertBackColor = GL::Color::parse(Util::rapidjson::getString(ui, "seifertBackColor"));
		if (ui.HasMember("isSeifertSurfaceVisible") && ui["isSeifertSurfaceVisible"].IsBool()) {
			this->isSeifertSurfaceVisible = std::make_shared<bool>(ui["isSeifertSurfaceVisible"].GetBool());
		}
	}
}

void KnotWrapper::moveSeifertBasePoint(double distance) {
	this->seifertStartPoint.move(
		GL::SeifertSurface::gradient(this->seifertStartPoint, this->snapshot()), distance
	);
	this->seifertSurface->destroy();
}

void KnotWrapper::toggleSeifertSurfaceVisibility() {
  if (this->seifertSurface->isVisible()) {
    this->isSeifertSurfaceVisible = std::make_shared<bool>(false);
  } else {
    this->isSeifertSurfaceVisible = std::make_shared<bool>(true);
  }
}

}}

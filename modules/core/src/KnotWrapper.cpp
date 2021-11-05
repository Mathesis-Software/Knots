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

#include "KnotWrapper.h"
#include "Diagram.h"
#include "KnotSurface.h"
#include "SeifertSurface.h"
#include "Util_rapidjson.h"

namespace KE::ThreeD {

namespace {

void initUnitaryMatrix(double matrix[3][3]) {
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			matrix[i][j] = i == j ? 1.0 : 0.0;
		}
	}
}

void rotateMatrix(double matrix[3][3], int axis0, int axis1, double angleDelta) {
	for (int i = 0; i < 3; ++i) {
		const double tmp = matrix[i][axis0] * sin(angleDelta) + matrix[i][axis1] * cos(angleDelta);
		matrix[i][axis0] = matrix[i][axis0] * cos(angleDelta) - matrix[i][axis1] * sin(angleDelta);
		matrix[i][axis1] = tmp;
	}
}

}// namespace

KnotWrapper::KnotWrapper(const TwoD::Diagram &diagram, std::size_t width, std::size_t height) : knot(Knot::pointsFromDiagram(diagram, width, height), diagram.caption) {
	initUnitaryMatrix(this->rotationMatrix);
	this->init();
}

KnotWrapper::KnotWrapper(const rapidjson::Value &doc) : knot(doc) {
	initUnitaryMatrix(this->rotationMatrix);
	this->readUiOptions(doc);
	this->init();
}

void KnotWrapper::init() {
	auto cp = this->serialize();
	this->setSaveCheckpoint(cp);
	this->_knotSurface = std::make_shared<GL::KnotSurface>(*this, 28);
	this->_seifertSurface = std::make_shared<GL::SeifertSurface>(*this);
}

rapidjson::Document KnotWrapper::serialize() const {
	auto doc = this->knot.serialize();
	this->saveUiOptions(doc);
	return doc;
}

void KnotWrapper::setSaveCheckpoint(const rapidjson::Value &doc) {
	this->saveCheckpoint = Util::rapidjson::docToString(doc);
}

bool KnotWrapper::isSaved() const {
	auto doc = this->serialize();
	return this->saveCheckpoint == Util::rapidjson::docToString(doc);
}

void KnotWrapper::saveUiOptions(rapidjson::Document &doc) const {
	rapidjson::Value componentUi(rapidjson::kObjectType);
	if (this->_knotColor) {
		const std::string sv = this->_knotColor->stringValue();
		rapidjson::Value color;
		color.SetString(sv.data(), sv.size(), doc.GetAllocator());
		componentUi.AddMember("color", color, doc.GetAllocator());
	}
	if (this->_knotThickness) {
		componentUi.AddMember("thickness", *this->_knotThickness, doc.GetAllocator());
	}
	doc["components"][0].AddMember("ui", componentUi, doc.GetAllocator());

	rapidjson::Value globalUi(rapidjson::kObjectType);

	rapidjson::Value matrix(rapidjson::kArrayType);
	for (int i = 0; i < 3; ++i) {
		rapidjson::Value row(rapidjson::kArrayType);
		for (int j = 0; j < 3; ++j) {
			row.PushBack(this->rotationMatrix[i][j], doc.GetAllocator());
		}
		matrix.PushBack(row, doc.GetAllocator());
	}
	globalUi.AddMember("matrix", matrix, doc.GetAllocator());

	if (this->_isSeifertSurfaceVisible) {
		globalUi.AddMember("isSeifertSurfaceVisible", *this->_isSeifertSurfaceVisible, doc.GetAllocator());
	}
	if (this->_seifertBasePoint) {
		rapidjson::Value basePoint(rapidjson::kObjectType);
		basePoint.AddMember("x", this->_seifertBasePoint->x, doc.GetAllocator());
		basePoint.AddMember("y", this->_seifertBasePoint->y, doc.GetAllocator());
		basePoint.AddMember("z", this->_seifertBasePoint->z, doc.GetAllocator());
		globalUi.AddMember("seifertBasePoint", basePoint, doc.GetAllocator());
	}
	if (this->_backgroundColor) {
		const std::string sv = this->_backgroundColor->stringValue();
		rapidjson::Value color;
		color.SetString(sv.data(), sv.size(), doc.GetAllocator());
		globalUi.AddMember("backgroundColor", color, doc.GetAllocator());
	}
	if (this->_seifertFrontColor) {
		const std::string sv = this->_seifertFrontColor->stringValue();
		rapidjson::Value color;
		color.SetString(sv.data(), sv.size(), doc.GetAllocator());
		globalUi.AddMember("seifertFrontColor", color, doc.GetAllocator());
	}
	if (this->_seifertBackColor) {
		const std::string sv = this->_seifertBackColor->stringValue();
		rapidjson::Value color;
		color.SetString(sv.data(), sv.size(), doc.GetAllocator());
		globalUi.AddMember("seifertBackColor", color, doc.GetAllocator());
	}
	doc.AddMember("ui", globalUi, doc.GetAllocator());
}

void KnotWrapper::readUiOptions(const rapidjson::Value &doc) {
	const auto &component = doc["components"][0];
	if (component.HasMember("ui") && component["ui"].IsObject()) {
		const auto &ui = component["ui"];
		this->_knotColor = Color::parse(Util::rapidjson::getString(ui, "color"));
		if (ui.HasMember("thickness") && ui["thickness"].IsNumber()) {
			this->_knotThickness = std::make_shared<double>(ui["thickness"].GetDouble());
		}
	}

	if (doc.HasMember("ui") && doc["ui"].IsObject()) {
		const auto &ui = doc["ui"];
		if (ui.HasMember("matrix") && ui["matrix"].IsArray() && ui["matrix"].Size() == 3) {
			const auto &matrix = ui["matrix"];
			for (int i = 0; i < 3; ++i) {
				const auto &row = matrix[i];
				if (row.IsArray() && row.Size() == 3) {
					for (int j = 0; j < 3; ++j) {
						if (row[j].IsNumber()) {
							this->rotationMatrix[i][j] = row[j].GetDouble();
						}
					}
				}
			}
		}
		this->_backgroundColor = Color::parse(Util::rapidjson::getString(ui, "backgroundColor"));
		this->_seifertFrontColor = Color::parse(Util::rapidjson::getString(ui, "seifertFrontColor"));
		this->_seifertBackColor = Color::parse(Util::rapidjson::getString(ui, "seifertBackColor"));
		if (ui.HasMember("isSeifertSurfaceVisible") && ui["isSeifertSurfaceVisible"].IsBool()) {
			this->_isSeifertSurfaceVisible = std::make_shared<bool>(ui["isSeifertSurfaceVisible"].GetBool());
		}
		if (ui.HasMember("seifertBasePoint") && ui["seifertBasePoint"].IsObject()) {
			const auto &basePoint = ui["seifertBasePoint"];
			if (basePoint.HasMember("x") && basePoint["x"].IsNumber() && basePoint.HasMember("y") && basePoint["y"].IsNumber() && basePoint.HasMember("z") && basePoint["z"].IsNumber()) {
				this->_seifertBasePoint = std::make_shared<Point>(basePoint["x"].GetDouble(), basePoint["y"].GetDouble(), basePoint["z"].GetDouble());
			}
		}
	}
}

std::shared_ptr<const GL::Surface> KnotWrapper::knotSurface() const {
	return this->_knotSurface;
}

std::shared_ptr<const GL::Surface> KnotWrapper::seifertSurface() const {
	return this->_seifertSurface;
}

Point KnotWrapper::seifertBasePoint() const {
	const auto ref = this->_seifertBasePoint;
	return ref ? *ref : Point(0.0, 0.0, 0.2);
}

void KnotWrapper::moveSeifertBasePoint(double distance) {
	Point basePoint = this->seifertBasePoint();
	basePoint.move(
					GL::SeifertSurface::gradient(basePoint, this->snapshot()), distance);
	this->_seifertBasePoint = std::make_shared<Point>(basePoint);
	this->_seifertSurface->destroy();
}

bool KnotWrapper::isSeifertSurfaceVisible() const {
	const auto ref = this->_isSeifertSurfaceVisible;
	return ref && *ref;
}

void KnotWrapper::setSeifertSurfaceVisibility(bool visible) {
	if (visible == this->isSeifertSurfaceVisible()) {
		return;
	}

	this->_isSeifertSurfaceVisible = std::make_shared<bool>(visible);
}

double KnotWrapper::knotThickness() const {
	const auto ref = this->_knotThickness;
	return ref ? *ref : 1.0;
}

void KnotWrapper::setKnotThickness(double thickness) {
	if (thickness != this->knotThickness()) {
		this->_knotThickness = std::make_shared<double>(thickness);
		this->_knotSurface->destroy();
	}
}

const Color &KnotWrapper::backgroundColor() const {
	const auto ref = this->_backgroundColor;
	return ref ? *ref : Color::white;
}

void KnotWrapper::setBackgroundColor(const Color &color) {
	if (color != this->backgroundColor()) {
		this->_backgroundColor = std::make_shared<Color>(color);
	}
}

const Color &KnotWrapper::knotColor() const {
	const auto ref = this->_knotColor;
	return ref ? *ref : Color::white;
}

void KnotWrapper::setKnotColor(const Color &color) {
	if (color != this->knotColor()) {
		this->_knotColor = std::make_shared<Color>(color);
	}
}

const Color &KnotWrapper::seifertFrontColor() const {
	const auto ref = this->_seifertFrontColor;
	return ref ? *ref : Color::white;
}

void KnotWrapper::setSeifertFrontColor(const Color &color) {
	if (color != this->seifertFrontColor()) {
		this->_seifertFrontColor = std::make_shared<Color>(color);
	}
}

const Color &KnotWrapper::seifertBackColor() const {
	const auto ref = this->_seifertBackColor;
	return ref ? *ref : Color::white;
}

void KnotWrapper::setSeifertBackColor(const Color &color) {
	if (color != this->seifertBackColor()) {
		this->_seifertBackColor = std::make_shared<Color>(color);
	}
}

void KnotWrapper::rotate(double dx, double dy, double dz) {
	if (dx != 0) {
		rotateMatrix(this->rotationMatrix, 2, 0, dx);
	}
	if (dy != 0) {
		rotateMatrix(this->rotationMatrix, 1, 2, dy);
	}
	if (dz != 0) {
		rotateMatrix(this->rotationMatrix, 0, 1, dz);
	}
}

}

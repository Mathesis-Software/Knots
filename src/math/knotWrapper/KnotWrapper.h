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

#ifndef __KNOTEDITOR_H__
#define __KNOTEDITOR_H__

#include "../knot/Knot.h"
#include "../surface/Surface.h"

namespace KE { namespace ThreeD {

class KnotWrapper {

private:
	Knot knot;
	std::string saveCheckpoint;

public:
	std::shared_ptr<GL::Color> backgroundColor;
	std::shared_ptr<GL::Color> knotColor;
	std::shared_ptr<double> knotThickness;
	std::shared_ptr<bool> isSeifertSurfaceVisible;
	std::shared_ptr<GL::Color> seifertFrontColor;
	std::shared_ptr<GL::Color> seifertBackColor;

public:
	KnotWrapper(const TwoD::Diagram &diagram, std::size_t width, std::size_t height);
	KnotWrapper(const rapidjson::Document &doc);

	Knot::Snapshot snapshot() const { return this->knot.snapshot(); }
	const std::string &caption() const { return this->knot.caption; }

	void decreaseEnergy() { this->knot.decreaseEnergy(); }
	void setCaption(const std::string &caption) { this->knot.caption = caption; } 
	void setLength(double length) { this->knot.setLength(length); }
	void center() { this->knot.center(); }
	void normalize(std::size_t numberOfPoints) { this->knot.normalize(numberOfPoints); }

	rapidjson::Document serialize(const double matrix[3][3]);
	bool isSaved(const double matrix[3][3]) const;

private:
	void saveUiOptions(rapidjson::Document &doc) const;
	void readUiOptions(const rapidjson::Document &doc);
};

}}

#endif /* __KNOTEDITOR_H__ */

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
#include "../surface/Color.h"

namespace KE::GL {

class Surface;
class KnotSurface;
class SeifertSurface;

}

namespace KE::ThreeD {

class KnotWrapper {

private:
	Knot knot;
	std::string saveCheckpoint;

	std::shared_ptr<Color> _backgroundColor;
	std::shared_ptr<Color> _knotColor;
	std::shared_ptr<double> _knotThickness;
	std::shared_ptr<bool> _isSeifertSurfaceVisible;
	std::shared_ptr<Color> _seifertFrontColor;
	std::shared_ptr<Color> _seifertBackColor;

	std::shared_ptr<GL::KnotSurface> _knotSurface;
	ThreeD::Point seifertStartPoint;
	std::shared_ptr<GL::SeifertSurface> _seifertSurface;

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

	std::shared_ptr<const GL::Surface> knotSurface() const;
	std::shared_ptr<const GL::Surface> seifertSurface() const;
	void moveSeifertBasePoint(double distance);
	bool isSeifertSurfaceVisible() const;
	void toggleSeifertSurfaceVisibility();

	double knotThickness() const;
	void setKnotThickness(double thickness);

	const Color &backgroundColor() const;
	void setBackgroundColor(const Color &color);
	const Color &knotColor() const;
	void setKnotColor(const Color &color);
	const Color &seifertFrontColor() const;
	void setSeifertFrontColor(const Color &color);
	const Color &seifertBackColor() const;
	void setSeifertBackColor(const Color &color);

	rapidjson::Document serialize(const double matrix[3][3]);
	bool isSaved(const double matrix[3][3]) const;

private:
	void init();
	void saveUiOptions(rapidjson::Document &doc) const;
	void readUiOptions(const rapidjson::Document &doc);
};

}

#endif /* __KNOTEDITOR_H__ */

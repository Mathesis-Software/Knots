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

#ifndef __SEIFERT_SURFACE_H__
#define __SEIFERT_SURFACE_H__

#include "../knotWrapper/KnotWrapper.h"
#include "../../gl/surface/Surface.h"

class seifert;

namespace KE { namespace GL {

class SeifertSurface : public Surface {

public:
	static ThreeD::Vector gradient(const ThreeD::Point &point, const ThreeD::Knot::Snapshot &snapshot);

private:
	std::shared_ptr<ThreeD::Knot::Snapshot> stored;

private:
	const ThreeD::KnotWrapper &base;
	const ThreeD::Point &startPoint;

	void addTriangles(seifert *s);
	void calculate() override;

public:
	SeifertSurface(const ThreeD::KnotWrapper &base, const ThreeD::Point &startPoint);

	const Color &frontColor() const override;
	const Color &backColor() const override;

	bool isObsolete() const override;
	bool isVisible() const override;
};

}}

#endif /* __SEIFERT_SURFACE_H__ */
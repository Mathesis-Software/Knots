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

#ifndef __SURFACE_H__
#define __SURFACE_H__

#include <vector>

#include "Color.h"
#include "primitives.h"

namespace KE::GL {

class Surface {

private:
	struct SurfacePoint {
		float vertex[3];
		float normal[3];

		SurfacePoint(float v0, float v1, float v2, float n0, float n1, float n2) {
			vertex[0] = v0;
			vertex[1] = v1;
			vertex[2] = v2;
			normal[0] = n0;
			normal[1] = n1;
			normal[2] = n2;
		}
	};

	const bool stripped;
	const bool showBackSide;

	mutable std::vector<SurfacePoint> points;

protected:
	virtual void calculate() const = 0;
	void addpoint(const ThreeD::Point &vertex, const ThreeD::Vector &normal) const;

public:
	Surface(bool stripped, bool showBackSide);
	virtual ~Surface();

	void paint() const;

	virtual bool isVisible() const = 0;
	virtual bool isObsolete() const;
	void destroy();

	virtual const Color &frontColor() const = 0;
	virtual const Color &backColor() const = 0;

private:
	Surface(const Surface&) = delete;
	Surface& operator = (const Surface&) = delete;
};

}

#endif /* __SURFACE_H__ */

/*
 * Copyright (c) 1995-2021, Nikolay Pultsin <geometer@geometer.name>
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

#ifndef __SEIFERT_SURFACE_H__
#define __SEIFERT_SURFACE_H__

#include "KnotWrapper.h"
#include "Surface.h"

namespace KE::GL {

class seifert;

class SeifertSurface : public Surface {

public:
	static ThreeD::Vector gradient(const ThreeD::Point &point, const ThreeD::Knot::Snapshot &snapshot);

private:
	mutable std::shared_ptr<ThreeD::Knot::Snapshot> stored;

private:
	const ThreeD::KnotWrapper &base;

public:
	SeifertSurface(const ThreeD::KnotWrapper &base);

private:
	void addTriangles(seifert *s) const;
	void calculate() const override;

	const Color &frontColor() const override;
	const Color &backColor() const override;

	bool isObsolete() const override;
	bool isVisible() const override;
};

}

#endif /* __SEIFERT_SURFACE_H__ */

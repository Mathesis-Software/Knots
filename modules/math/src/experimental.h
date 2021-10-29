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

#ifndef __EXPERIMENTAL_H__
#define __EXPERIMENTAL_H__

#include "computable.h"

namespace KE::ThreeD {

class Knot;

}

namespace KE::ThreeD::Math {

class Experimental : public Computable {

private:
	double compute(const Knot::Snapshot &snapshot) override;

public:
	Experimental(const KnotWrapper &knot);
};

class Singular : public Computable {

private:
	double compute(const Knot::Snapshot &snapshot) override;

public:
	Singular(const KnotWrapper &knot);
};

class Experimental2 : public Computable {

public:
	const int order;

private:
	double compute(const Knot::Snapshot &snapshot) override;

public:
	Experimental2(const KnotWrapper &knot, int order);
};

}

#endif /* __EXPERIMENTAL_H__ */

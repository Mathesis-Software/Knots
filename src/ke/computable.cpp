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

#include "computable.h"
#include "KnotWrapper.h"

namespace KE::ThreeD::Computables {

double Computable::value() {
	if (!this->snapshot || this->snapshot->isObsolete()) {
		this->snapshot = std::make_shared<Knot::Snapshot>(this->knot.snapshot());
		this->_value = this->compute(*this->snapshot);
	}

	return this->_value;
}

}

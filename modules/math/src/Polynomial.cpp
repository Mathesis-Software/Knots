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

#include "Polynomial.h"

namespace KE::TwoD::Math {

Polynomial Polynomial::T = Polynomial({0, 1});
Polynomial Polynomial::MINUS_T = Polynomial({0, -1});
Polynomial Polynomial::ONE = Polynomial({1});
Polynomial Polynomial::MINUS_ONE = Polynomial({-1});
Polynomial Polynomial::ZERO = Polynomial();

Polynomial Polynomial::reduced() const {
	std::vector<int> reduced;
	bool skip = true;
	for (const auto &coef : this->coefficients) {
		if (coef == 0 && skip) {
			continue;
		}
		skip = false;
		reduced.push_back(coef);
	}
	Polynomial result(reduced);
	if (!reduced.empty() && reduced[0] < 0) {
		result *= -1;
	}
	return result;
}

std::ostream &operator<<(std::ostream &os, const Polynomial &poly) {
	if (poly.coefficients.empty()) {
		os << 0;
		return os;
	}
	if (poly.coefficients.size() == 1) {
		os << poly.coefficients[0];
		return os;
	}

	for (int index = poly.coefficients.size() - 1; index >= 0; index -= 1) {
		const auto coef = poly.coefficients[index];
		switch (coef) {
			case 0:
				continue;
			case 1:
				if (index != (int)poly.coefficients.size() - 1) {
					os << " + ";
				}
				if (index == 0) {
					os << 1;
				}
				break;
			case -1:
				if (index > 0) {
					os << " - ";
				} else {
					os << "- 1";
				}
				break;
			default:
				if (index != (int)poly.coefficients.size() - 1) {
					if (coef > 0) {
						os << " + ";
					} else {
						os << " - ";
					}
				}
				os << abs(coef);
				break;
		}
		switch (index) {
			case 0:
				break;
			case 1:
				os << "t";
				break;
			default:
				os << "t^" << index;
				break;
		}
	}
	return os;
}

}

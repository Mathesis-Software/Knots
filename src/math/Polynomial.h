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

#ifndef __KE_MATH_POLYNOMIAL_H__
#define __KE_MATH_POLYNOMIAL_H__

#include <iostream>
#include <vector>

namespace KE::TwoD::Math {

class Polynomial {

public:
	static Polynomial T;
	static Polynomial MINUS_T;
	static Polynomial ONE;
	static Polynomial MINUS_ONE;
	static Polynomial ZERO;

private:
	std::vector<int> coefficients;

public:
	Polynomial() {}

private:
	Polynomial(const std::vector<int> &coefficients) : coefficients(coefficients) {
		this->normalize();
	}

	void normalize() {
		while (this->coefficients.size() > 0 && this->coefficients.back() == 0) {
			this->coefficients.pop_back();
		}
	}

public:
	const Polynomial &operator += (const Polynomial &poly) {
		while (this->coefficients.size() < poly.coefficients.size()) {
			this->coefficients.push_back(0);
		}
		for (std::size_t index = 0; index < poly.coefficients.size(); index += 1) {
			this->coefficients[index] += poly.coefficients[index];
		}
		this->normalize();
		return *this;
	}

	const Polynomial &operator *= (int num) {
		if (num == 0) {
			this->coefficients.empty();
			return *this;
		}
		std::transform(this->coefficients.begin(), this->coefficients.end(), this->coefficients.begin(), [num](int co) {return co * num;});
		return *this;
	}

	Polynomial operator *(const Polynomial &poly) {
		if (this->coefficients.empty() || poly.coefficients.empty()) {
			return Polynomial::ZERO;
		}
		std::vector<int> product(this->coefficients.size() + poly.coefficients.size() - 1, 0);
		for (std::size_t i = 0; i < this->coefficients.size(); i += 1) {
			for (std::size_t j = 0; j < poly.coefficients.size(); j += 1) {
				product[i + j] += this->coefficients[i] * poly.coefficients[j];
			}
		}
		return Polynomial(product);
	}

	bool operator == (int num) const {
		if (this->coefficients.empty()) {
			return num == 0;
		} else if (this->coefficients.size() == 1) {
			return num == this->coefficients[0];
		} else {
			return false;
		}
	}
	bool operator != (int num) const {
		return !(*this == num);
	}

	Polynomial reduced() const;

	friend std::ostream &operator << (std::ostream &os, const Polynomial &poly);
};

}

#endif /* __KE_MATH_POLYNOMIAL_H__ */

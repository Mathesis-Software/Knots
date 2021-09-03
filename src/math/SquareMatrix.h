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

#ifndef __KE_MATH_SQUARE_MATRIX_H__
#define __KE_MATH_SQUARE_MATRIX_H__

#include <vector>

namespace KE::TwoD::Math {

template<typename T>
class SquareMatrix {

public:
	virtual ~SquareMatrix() {}

	virtual std::size_t dimension() const = 0;
	virtual const T &at(std::size_t i, std::size_t j) const = 0;

	T determinant() const;
};

template<typename T>
class ConstMatrix : public SquareMatrix<T> {

private:
	const std::vector<std::vector<T>> elements;

public:
	ConstMatrix(const std::vector<std::vector<T>> &elements) : elements(elements) {
	}

	std::size_t dimension() const override {
		return this->elements.size();
	}
	const T &at(std::size_t i, std::size_t j) const override {
		return this->elements[i][j];
	}
};

template<typename T>
class SliceMatrix : public SquareMatrix<T> {

private:
	const SquareMatrix<T> &base;
	const std::size_t row, column;

public:
	SliceMatrix(const SquareMatrix<T> &base, std::size_t row, std::size_t column) : base(base), row(row), column(column) {
	}

	std::size_t dimension() const override {
		return this->base.dimension() - 1;
	}
	const T &at(std::size_t i, std::size_t j) const override {
		if (i >= this->row) {
			i += 1;
		}
		if (j >= this->column) {
			j += 1;
		}
		return this->base.at(i, j);
	}
};

template<typename T>
T SquareMatrix<T>::determinant() const {
	const std::size_t dim = this->dimension();
	if (dim == 1) {
		return this->at(0, 0);
	} else {
		T sum;
		for (std::size_t i = 0; i < dim; i += 1) {
			T coef = this->at(i, 0);
			if (coef != 0) {
				if (i % 2) {
					coef *= -1;
				}
				sum += coef * SliceMatrix<T>(*this, i, 0).determinant();
			}
		}
		return sum;
	}
}

}

#endif /* __KE_MATH_SQUARE_MATRIX_H__ */

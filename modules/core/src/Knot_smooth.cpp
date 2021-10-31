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

#include <cmath>

#include "Knot.h"

namespace KE::ThreeD {

// Узел перемещается так, чтобы его центр масс оказался
// в начале координат.
void Knot::center() {
	std::lock_guard<std::mutex> writeMethodGuard(this->writeMethodMutex);
	counting_lock guard(*this);

	Vector delta(0.0, 0.0, 0.0);

	for (const Point &pt : this->_points) {
		delta.x += pt.x;
		delta.y += pt.y;
		delta.z += pt.z;
	}

	delta.x /= this->_points.size();
	delta.y /= this->_points.size();
	delta.z /= this->_points.size();

	for (Point &pt : this->_points) {
		pt.move(delta, -1);
	}
}

// Длина ломаной устанавливается равной len.
void Knot::setLength(double len) {
	std::lock_guard<std::mutex> writeMethodGuard(this->writeMethodMutex);
	double ratio = len / this->snapshot().knotLength();

	counting_lock guard(*this);
	for (Point &pt : this->_points) {
		pt.x *= ratio;
		pt.y *= ratio;
		pt.z *= ratio;
	}
}

void Knot::decreaseEnergy() {
	std::lock_guard<std::mutex> writeMethodGuard(this->writeMethodMutex);

	// Сохраняем длину кривой, чтобы в конце восстановить ее.
	const auto snapshot = this->snapshot();
	const double totalLength = snapshot.knotLength();

	// Расставляем точки на кривой равномерно.
	auto points = this->normalizedPoints(snapshot, snapshot.size());

	std::vector<Vector> edges;
	edges.reserve(points.size());
	for (std::size_t i = 0; i < points.size() - 1; ++i) {
		edges.push_back(Vector(points[i], points[i + 1]));
	}
	edges.push_back(Vector(points.back(), points.front()));

	// Создаем массив расстояний между соседними точками.
	std::vector<double> lengths;
	lengths.reserve(edges.size());
	for (const auto &edge : edges) {
		lengths.push_back(edge.length());
	}

	std::vector<Vector> delta;

	// Вычисляем вектор градиента в каждой вершине ломаной.
	for (std::size_t i = 0; i < points.size(); i++) {
		// Создаем вектор градиента для p_i.
		Vector delta_i(0.0, 0.0, 0.0);
		// Вычисляем общие коэффициенты для всех слагаемых в p_i.
		const double lt = lengths[i] + lengths[snapshot.prev(i)];
		const Vector local = Vector::linear(
						edges[i], -1 / lengths[i],
						edges[snapshot.prev(i)], 1 / lengths[snapshot.prev(i)]);

		for (std::size_t j = snapshot.next(i); j != snapshot.prev(i); j = snapshot.next(j)) {
			// Ищем ближайшую к p_i точку на ребре p_jp_{j+1}:
			//	 если -xr / r2 < 0, это p_j,
			//	 если -xr / r2 > 1 -- p_{j+1},
			//	 иначе -- точка внутри ребра.
			Vector x(points[i], points[j]);
			double xr = x.scalar_product(edges[j]);
			double r2 = lengths[j] * lengths[j];
			double x2 = x.square();

			// Записываем в x[] вектор от p_i до ближайшей точки,
			// а в x2 -- квадрат его длины.
			if (xr + r2 < 0.0) {
				x2 += r2 + 2 * xr;
				x.add(edges[j]);
			} else if (xr < 0.0) {
				double tau = xr / r2;
				x2 -= tau * xr;
				x.add(edges[j], -tau);
			}

			// Добавляем к градиенту в p_i слагаемое от взаимодействия с p_jp_{j+1}.
			delta_i.x -= lengths[j] / x2 * (x.x * lt / x2 + local.x);
			delta_i.y -= lengths[j] / x2 * (x.y * lt / x2 + local.y);
			delta_i.z -= lengths[j] / x2 * (x.z * lt / x2 + local.z);
		}
		delta.push_back(delta_i);
	}

	// Находим максимум из длин (компонент) градиентов.
	double max_shift = 0.0;
	for (const Vector &d : delta) {
		max_shift = std::max(max_shift, fabs(d.x));
		max_shift = std::max(max_shift, fabs(d.y));
		max_shift = std::max(max_shift, fabs(d.z));
	}

	// Вычисляем коэффициент, на который нужно домножить градиент.
	double coeff = totalLength * totalLength / points.size() / points.size() / 10.0;
	if (coeff > totalLength / points.size() / max_shift / 5.0)
		coeff = totalLength / points.size() / max_shift / 5.0;

	// Делаем сдвиг в направлении градиента.
	for (std::size_t i = 0; i < points.size(); i++) {
		points[i].move(delta[i], coeff);
	}

	double newLength = points.back().distanceTo(points.front());
	for (std::size_t i = 0; i < points.size() - 1; ++i) {
		newLength += points[i].distanceTo(points[i + 1]);
	}

	const double ratio = totalLength / newLength;
	for (auto &pt : points) {
		pt.x *= ratio;
		pt.y *= ratio;
		pt.z *= ratio;
	}

	Vector shift(0.0, 0.0, 0.0);
	for (const auto &pt : points) {
		shift.x += pt.x;
		shift.y += pt.y;
		shift.z += pt.z;
	}
	for (auto &pt : points) {
		pt.move(shift, -1.0 / points.size());
	}

	counting_lock guard(*this);
	this->_points.swap(points);
}

}// namespace KE::ThreeD

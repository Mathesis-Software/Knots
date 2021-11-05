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

#include "seifert.h"
#include "SeifertSurface.h"

namespace KE::GL {

namespace {

const int MAX_ITERATION_NUMBER = 20; // Максимальное число итераций при
																		 // добавлении новой точки в заданном
						 // направлении.
const double MIN_TAU = 0.0001;			 // Итерации при добавлении точки в
																		 // заданном направлении прекращаются,
																		 // если скалярное произведение вектора
						 // сдвига и суммы градиентов на его
						 // концах не превосходит этого числа.
const int MAX_POINTS_NUMBER = 8000;	// Максимальное число точек поверхности.
const double MIN_EPS = 0.024;				// Минимальное и максимальное значения
const double MAX_EPS = 0.04;				 // допустимого сдвига.
const double NEAR = 1.5;						 // Соседними считаются точки, находящиеся
																		 // друг от друга
						 // на расстоянии <= NEAR * Eps.
const double TIMES = 2.0;						// Значение допустимого сдвига не
																		 // превосходит расстояния до узла,
						 // деленного на TIMES.


int counter = 0;

/*
 * The square of the distance from point to the segment [pt0; pt1]
 */
double distance2(const ThreeD::Point &point, const ThreeD::Point &pt0, const ThreeD::Point &pt1) {
	const ThreeD::Vector x(point, pt0);
	const ThreeD::Vector r(pt0, pt1);
	const double xr = x.scalar_product(r);
	const double x2 = x.square();
	const double r2 = r.square();
	const double tau = - xr / r2;

	if (tau < 0.0) {
		return x2;
	} else if (tau > 1.0) {
		return x2 + r2 + 2 * xr;
	} else {
		return x2 + tau * xr;
	}
}

double distance(const ThreeD::Point &point, const ThreeD::Knot::Snapshot &snapshot) {
	double d2 = std::numeric_limits<float>::max();

	for (std::size_t i = 0; i < snapshot.size(); i++) {
		d2 = std::min(d2, distance2(point, snapshot[i], snapshot[snapshot.next(i)]));
	}

	return sqrt(d2);
}

}

void seifert::addPoint(const ThreeD::Vector &direction) {
	// mutable version of direction
	ThreeD::Vector dir(direction);
	dir.normalize();

	// Создаем новую точку в заданном направлении от текущей
	// на расстоянии localEps.

	double tau = 1.0;

	// Делаем несколько последовательных приближений.
	for (int i = 0; fabs(tau) > MIN_TAU && i < MAX_ITERATION_NUMBER; ++i) {
		// Считаем приближение для новой точки.
		ThreeD::Point appr(this->point);
		appr.move(dir, localEps);

		// Считаем сумму градиентов в начальной точке и
		// в приближении новой точки.
		ThreeD::Vector gradient2 = SeifertSurface::gradient(appr, this->snapshot);
		gradient2.add(this->gradient);

		// Подправляем направление, чтобы оно стало перпендикулярно
		// сумме градиентов.
		tau = dir.scalar_product(gradient2) / gradient2.square();
		dir.add(gradient2, -tau);
		dir.normalize();
	}

	// Считаем последнее приближение и добавляем точку.
	ThreeD::Point new_point(point);
	new_point.move(dir, localEps);
	new seifert(this->snapshot, new_point, this);
}

void seifert::addPoint60(const ThreeD::Vector &direction) {
	// Создаем новую точку в направлении, составляющем 60^o
	// с направлением dir, перпендикулярном градиенту в данной точке
	// и составляющем с этими направлениями тройку фиксированного знака.
	// (Предполагается, что данное направление почти перпендикулярно
	// градиенту.)
	ThreeD::Vector dir(direction);
	dir.normalize();
	ThreeD::Vector orth = dir.vector_product(this->gradient);
	orth.normalize();
	dir.add(orth, sqrt(3));
	this->addPoint(dir);
}

void seifert::searchForNeighbor() {
	// Здесь ищутся соседи среди уже существующих точек
	// (просто просматривается весь список).

	seifert_ord *o;

	for (o = sord->next;
			 o && (o->value->point.x - this->point.x < NEAR * localEps);
			 o = o->next) {
		const ThreeD::Vector coor(o->value->point, this->point);
		ThreeD::Vector grad(this->gradient);
		grad.add(o->value->gradient);
		const double dist2 = coor.square();
		const double grad2 = grad.square();
		const double scal = coor.scalar_product(grad);

		// Проверяем расстояние и ``почти перпендикулярность'' градиенту,
		// добавляем точки в окрестности друг друга, если нужно.
		if (dist2 < NEAR * NEAR * localEps * localEps && scal * scal < 0.1 * dist2 * grad2) {
			neighborhood->insert(o->value);
			o->value->neighborhood->insert(this);
		}
	}

	for (o = sord->prev;
			 o && (this->point.x - o->value->point.x < NEAR * localEps);
			 o = o->prev) {
		const ThreeD::Vector coor(o->value->point, this->point);
		ThreeD::Vector grad(this->gradient);
		grad.add(o->value->gradient);
		const double dist2 = coor.square();
		const double grad2 = grad.square();
		const double scal = coor.scalar_product(grad);

		// Проверяем расстояние и ``почти перпендикулярность'' градиенту,
		// добавляем точки в окрестности друг друга, если нужно.
		if (dist2 < NEAR * NEAR * localEps * localEps && scal * scal < 0.1 * dist2 * grad2) {
			neighborhood->insert(o->value);
			o->value->neighborhood->insert(this);
		}
	}
}

void seifert::checkNeighborhood() {
	// Проверяем, не нужно ли здесь остановиться по внешним причинам.
	// (Выход за границы области видимости, добрались до узла и т.п.)
	if (this->point.x * this->point.x + this->point.y * this->point.y + this->point.z * this->point.z > 2.0) {
		return;
	}

	// Если соседей вообще нет (т.е. это просто первая точка),
	// то создаем первую точку в направлении, перпендикулярном градиенту.
	if (!neighborhood->value) {
		if (fabs(gradient.x) > fabs(gradient.y) && fabs(gradient.x) > fabs(gradient.z)) {
			this->addPoint(ThreeD::Vector(- gradient.y, gradient.x, 0));
		} else {
			this->addPoint(ThreeD::Vector(0, - gradient.z, gradient.y));
		}
	}

	// Если сосед ровно один, создаем второго соседа, чтобы
	// точка и 2 ее соседа образовывали равносторонний треугольник.
	if (neighborhood->next == neighborhood) {
		this->addPoint60(ThreeD::Vector(this->point, neighborhood->value->point));
	}

	// Окрестность проверяется на предмет наличия ``зазоров''
	// (углов > 90^o между направлениями на соседей), если таковые есть,
	// то они заполняются (вставляется новая точка на расстоянии localEps от
	// данной, чтобы угол между направлениями на новую точку и на первого
	// из соседей был равен 60^o.
	{
		seifert_list *start = neighborhood;
		for (;;) {
			// Это векторы направления на двух соседей.
			const ThreeD::Vector vect1(this->point, neighborhood->value->point);
			const ThreeD::Vector vect2(this->point, neighborhood->next->value->point);

			if (vect1.scalar_product(vect2) < 0 ||
					this->gradient.scalar_product(vect1.vector_product(vect2)) > 0.001 * MAX_EPS * MAX_EPS) {
				this->addPoint60(vect1);
			}

			if (counter > MAX_POINTS_NUMBER - 1) {
				break;
			}
			neighborhood = neighborhood->next;
			if (neighborhood == start) {
				break;
			}
		}
	}
}

seifert_list *seifert::hasNeighbor(seifert *n) {
	for (seifert_list *sl = neighborhood; ; sl = sl->next) {
		if (sl->value == n)
			return sl;
		if (sl->next == neighborhood)
			return nullptr;
	}
}

void seifert::markUsed(seifert *start, seifert *end) {
	seifert_list *sl_start = hasNeighbor(start);
	if (!sl_start)
		return;

	seifert_list *sl_end = hasNeighbor(end);
	if (!sl_end)
		return;

	for (; sl_start != sl_end; sl_start = sl_start->next)
		sl_start->label = 1;
}

void seifert::correction_local() {
	if (!neighborhood->value)
		return;

	for (seifert_list *sl = neighborhood; ; sl = sl->next) {
		sl->value->neighborhood->insert(sl->next->value);
		sl->next->value->neighborhood->insert(sl->value);
		if (sl->next == neighborhood)
			break;
	}
}

void seifert::correction() {
	correction_local();

	seifert_ord *so = sord;
	while (so->prev) {
		so = so->prev;
		so->value->correction_local();
	}

	so = sord;
	while (so->next) {
		so = so->next;
		so->value->correction_local();
	}
}

seifert::seifert(const ThreeD::Knot::Snapshot &snapshot, const ThreeD::Point &point, seifert *neighbor) : snapshot(snapshot), point(point), gradient(SeifertSurface::gradient(point, snapshot)) {
	counter ++;
//	cerr << "Point " << counter << '\n';

	localEps = std::min(distance(this->point, this->snapshot) / TIMES, MAX_EPS);

	if (neighbor)
		sord = neighbor->sord->insert(this);
	else
		sord = new seifert_ord(this);

	neighborhood = new seifert_list(this, neighbor);
	if (neighbor) {
		neighbor->neighborhood->insert_after(this);
		searchForNeighbor();
	}

	if (localEps > MIN_EPS) {
		checkNeighborhood();
	}
}

seifert::~seifert() {
	if (sord->next) {
		sord->next->prev = nullptr;
		delete sord->next->value;
		sord->next = nullptr;
	}
	if (sord->prev) {
		sord->prev->next = nullptr;
		delete sord->prev->value;
		sord->prev = nullptr;
	}
	delete sord;
	delete neighborhood;

	counter--;
}

}

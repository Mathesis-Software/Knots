#include <cmath>

#include "seifert.h"
#include "../knot/knot.h"

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


static int counter = 0;

void seifert::addPoint(const KE::ThreeD::Vector &direction) {
	// mutable version of direction
	KE::ThreeD::Vector dir(direction);

	// Создаем новую точку в заданном направлении от текущей
	// на расстоянии localEps.

	double tau = 1.0;

	// Делаем несколько последовательных приближений.
	for (int i = 0; fabs(tau) > MIN_TAU && i < MAX_ITERATION_NUMBER; ++i) {
		// Считаем приближение для новой точки.
		const double eps = dir.length() / localEps;

		KE::ThreeD::Point appr(this->coord);
		appr.move(dir, 1 / eps);

		// Считаем сумму градиентов в начальной точке и
		// в приближении новой точки.
		KE::ThreeD::Vector gradient2 = this->base.seifertGradient(appr);
		gradient2.add(this->gradient);

		// Подправляем направление, чтобы оно стало перпендикулярно
		// сумме градиентов.
		tau = dir.scalar_product(gradient2) / gradient2.square();
		dir.add(gradient2, -tau);
	}

	// Считаем последнее приближение и добавляем точку.
	const double eps = dir.length() / localEps;
	new seifert(coord.x + dir.x / eps, coord.y + dir.y / eps, coord.z + dir.z / eps, base, this);
}

void seifert::addPoint60(const KE::ThreeD::Vector &direction) {
	// Создаем новую точку в направлении, составляющем 60^o
	// с направлением dir, перпендикулярном градиенту в данной точке
	// и составляющем с этими направлениями тройку фиксированного знака.
	// (Предполагается, что данное направление почти перпендикулярно
	// градиенту.)
	KE::ThreeD::Vector dir(direction);
	dir.normalize();
	KE::ThreeD::Vector orth = dir.vector_product(this->gradient);
	orth.normalize();
	dir.add(orth, sqrt(3));
	this->addPoint(dir);
}

void seifert::searchForNeighbor() {
	// Здесь ищутся соседи среди уже существующих точек
	// (просто просматривается весь список).

	seifert_ord *o;

	for (o = sord->next;
			 o && (o->value->coord.x - coord.x < NEAR * localEps);
			 o = o->next) {
		const KE::ThreeD::Vector coor(o->value->coord, this->coord);
		KE::ThreeD::Vector grad(this->gradient);
		grad.add(o->value->gradient);
		const double dist2 = coor.square();
		const double grad2 = grad.square();
		const double scal	= coor.scalar_product(grad);

		// Проверяем расстояние и ``почти перпендикулярность'' градиенту,
		// добавляем точки в окрестности друг друга, если нужно.
		if (dist2 < NEAR * NEAR * localEps * localEps && scal * scal < 0.1 * dist2 * grad2) {
			neighborhood->insert(o->value);
			o->value->neighborhood->insert(this);
		}
	}

	for (o = sord->prev;
			 o && (coord.x - o->value->coord.x < NEAR * localEps);
			 o = o->prev) {
		const KE::ThreeD::Vector coor(o->value->coord, this->coord);
		KE::ThreeD::Vector grad(this->gradient);
		grad.add(o->value->gradient);
		const double dist2 = coor.square();
		const double grad2 = grad.square();
		const double scal	= coor.scalar_product(grad);

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
	if (coord.x * coord.x + coord.y * coord.y + coord.z * coord.z > 2.0) {
		return;
	}

	// Если соседей вообще нет (т.е. это просто первая точка),
	// то создаем первую точку в направлении, перпендикулярном градиенту.
	if (!neighborhood->value) {
		if (fabs(gradient.x) > fabs(gradient.y) && fabs(gradient.x) > fabs(gradient.z)) {
			this->addPoint(KE::ThreeD::Vector(- gradient.y, gradient.x, 0));
		} else {
			this->addPoint(KE::ThreeD::Vector(0, - gradient.z, gradient.y));
		}
	}

	// Если сосед ровно один, создаем второго соседа, чтобы
	// точка и 2 ее соседа образовывали равносторонний треугольник.
	if (neighborhood->next == neighborhood) {
		this->addPoint60(KE::ThreeD::Vector(this->coord, neighborhood->value->coord));
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
			const KE::ThreeD::Vector vect1(this->coord, neighborhood->value->coord);
			const KE::ThreeD::Vector vect2(this->coord, neighborhood->next->value->coord);

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
			return NULL;
	}
}

void seifert::markUsed(seifert *start, seifert *end) {
	seifert_list *sl_start = hasNeighbor (start);
	if (!sl_start)
		return;

	seifert_list *sl_end = hasNeighbor (end);
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

seifert::seifert(const double x, const double y, const double z,
								 const KE::ThreeD::Knot &base, seifert* neighbor) : base(base), coord(x, y, z), gradient(base.seifertGradient(coord)) {
	counter ++;
//	cerr << "Point " << counter << '\n';

	localEps = this->base.minDist(coord) / TIMES;
	if (localEps > MAX_EPS)
		localEps = MAX_EPS;

	if (neighbor)
		sord = neighbor->sord->insert(this);
	else
		sord = new seifert_ord(this);

	neighborhood = new seifert_list(this, neighbor);
	if (neighbor) {
		neighbor->neighborhood->insert_after(this);
		searchForNeighbor ();
	}

	if (localEps > MIN_EPS) {
		checkNeighborhood();
	}
}

seifert::~seifert() {
	if (sord->next)
	{
		sord->next->prev = NULL;
		delete sord->next->value;
		sord->next = NULL;
	}
	if (sord->prev)
	{
		sord->prev->next = NULL;
		delete sord->prev->value;
		sord->prev = NULL;
	}
	delete sord;
	delete neighborhood;

	counter --;
}

#include <math.h>
#include "knot.h"

namespace KE { namespace ThreeD {

// Узел перемещается так, чтобы его центр масс оказался
// в начале координат.
void Knot::center() {
	std::lock_guard<std::recursive_mutex> guard(this->mutex);
	Vector delta(0.0, 0.0, 0.0);

  for (const Point &pt : this->points) {
    delta.x += pt.x;
    delta.y += pt.y;
    delta.z += pt.z;
  }

  delta.x /= this->points.size();
  delta.y /= this->points.size();
  delta.z /= this->points.size();

  for (Point &pt : this->points) {
		pt.move(delta, -1);
  }
}

// Длина ломаной устанавливается равной len.
void Knot::setLength(double len) {
	std::lock_guard<std::recursive_mutex> guard(this->mutex);
  len /= this->length->value();

  for (Point &pt : this->points) {
    pt.x *= len;
    pt.y *= len;
    pt.z *= len;
	}

  // Все параметры придется пересчитывать.
  clear_depend ();
}

void Knot::decreaseEnergy() {
	std::lock_guard<std::recursive_mutex> guard(this->mutex);
  // Сохраняем длину кривой, чтобы в конце восстановить ее.
  double oldLen = this->length->value();

  // Расставляем точки на кривой равномерно.
  normalize(this->points.size());

  // Создаем массив расстояний между соседними точками.
	const auto len_table = this->len_table();

  double lt, local[3];
	std::vector<Vector> delta;

  // Вычисляем вектор градиента в каждой вершине ломаной.
  for (std::size_t i = 0; i < this->points.size(); i++) {
    // Создаем вектор градиента для p_i.
    Vector delta_i(0.0, 0.0, 0.0);
    // Вычисляем общие коэффициенты для всех слагаемых в p_i.
    lt = len_table[i] + len_table[prev(i)];
		local[0] =
			(points[i].x - points[next(i)].x) / len_table[i] +
			(points[i].x - points[prev(i)].x) / len_table[prev(i)];
		local[1] =
			(points[i].y - points[next(i)].y) / len_table[i] +
			(points[i].y - points[prev(i)].y) / len_table[prev(i)];
		local[2] =
			(points[i].z - points[next(i)].z) / len_table[i] +
			(points[i].z - points[prev(i)].z) / len_table[prev(i)];

    for (std::size_t j = next(i); j != prev(i); j = next(j)) {
      // Ищем ближайшую к p_i точку на ребре p_jp_{j+1}:
      //   если -xr / r2 < 0, это p_j,
      //   если -xr / r2 > 1 -- p_{j+1},
      //   иначе -- точка внутри ребра.
			Vector x(points[i], points[j]);
			const Vector r(points[j], points[next(j)]);
			double xr = x.scalar_product(r);
      double r2 = len_table[j] * len_table[j];
      double x2 = x.square();

      // Записываем в x[] вектор от p_i до ближайшей точки,
      // а в x2 -- квадрат его длины.
      if (xr + r2 < 0.0) {
        x2 += r2 + 2 * xr;
				x.add(r);
      } else if (xr < 0.0) {
        double tau = xr / r2;
        x2 -= tau * xr;
				x.add(r, -tau);
      }

      // Добавляем к градиенту в p_i слагаемое от взаимодействия с p_jp_{j+1}.
      delta_i.x -= len_table[j] / x2 * (x.x * lt / x2 + local[0]);
      delta_i.y -= len_table[j] / x2 * (x.y * lt / x2 + local[1]);
      delta_i.z -= len_table[j] / x2 * (x.z * lt / x2 + local[2]);
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
  double coeff = oldLen * oldLen / this->points.size() / this->points.size() / 10.0;
  if (coeff > oldLen / this->points.size() / max_shift / 5.0)
    coeff = oldLen / this->points.size() / max_shift / 5.0;

  // Делаем сдвиг в направлении градиента.
  for (std::size_t i = 0; i < this->points.size(); i++) {
    points[i].move(delta[i], coeff);
	}

  // Ломаная изменилась, так что все параметры придется пересчитывать.
  // Удаляем старые значения.
  clear_depend();

  // Восстанавливаем старую длину.
  setLength(oldLen);
}

}}

#include <math.h>
#include "knot.h"

void Knot::center() {
  // Узел перемещается так, чтобы его центр масс оказался
  // в начале координат.

  double deltaX = 0.0,
         deltaY = 0.0,
         deltaZ = 0.0;

  for (std::size_t i = 0; i < this->points.size(); i++) {
    deltaX += points[i].x;
    deltaY += points[i].y;
    deltaZ += points[i].z;
  }

  deltaX /= this->points.size();
  deltaY /= this->points.size();
  deltaZ /= this->points.size();

  for (std::size_t i = 0; i < this->points.size(); i++) {
    points[i].x -= deltaX;
    points[i].y -= deltaY;
    points[i].z -= deltaZ;
  }
}

void Knot::setLength(double len) {
  // Длина ломаной устанавливается равной len.

  len /= this->length->value();

  for (std::size_t i = 0; i < this->points.size(); i++) {
    points[i].x *= len;
    points[i].y *= len;
    points[i].z *= len;
	}

  // Все параметры придется пересчитывать.
  clear_depend ();
}

void Knot::decreaseEnergy() {
  // Сохраняем длину кривой, чтобы в конце восстановить ее.
  double oldLen = this->length->value();

  // Расставляем точки на кривой равномерно.
  normalize(this->points.size());

  // Создаем массив расстояний между соседними точками.
	const auto &len_table = this->len_table();

	std::size_t i, j, k;
  double xr, x2, r2, tau, lt, x[3], r[3], local[3];
  double **delta = new double*[this->points.size()];

  // Вычисляем вектор градиента в каждой вершине ломаной.
  for (i = 0; i < this->points.size(); i++) {
    // Создаем и обнуляем вектор градиента для p_i.
    delta[i] = new double[3];
    for (k = 0; k < 3; k++)
      delta[i][k] = 0.0;
    // Вычисляем общие коэффициенты для всех слагаемых в p_i.
    lt = len_table[i] + len_table[prev (i)];
		local[0] =
			(points[i].x - points[next (i)].x) / len_table[i] +
			(points[i].x - points[prev (i)].x) / len_table[prev (i)];
		local[1] =
			(points[i].y - points[next (i)].y) / len_table[i] +
			(points[i].y - points[prev (i)].y) / len_table[prev (i)];
		local[2] =
			(points[i].z - points[next (i)].z) / len_table[i] +
			(points[i].z - points[prev (i)].z) / len_table[prev (i)];

    for (j = next (i); j != prev (i); j = next (j))
    {
      // Ищем ближайшую к p_i точку на ребре p_jp_{j+1}:
      //   если -xr / r2 < 0, это p_j,
      //   если -xr / r2 > 1 -- p_{j+1},
      //   иначе -- точка внутри ребра.
      x[0] = points[j].x - points[i].x;
      x[1] = points[j].y - points[i].y;
      x[2] = points[j].z - points[i].z;
      r[0] = points[next (j)].x - points[j].x;
      r[1] = points[next (j)].y - points[j].y;
      r[2] = points[next (j)].z - points[j].z;
      xr = x[0] * r[0] + x[1] * r[1] + x[2] * r[2];
      r2 = len_table[j] * len_table[j];
      x2 = x[0] * x[0] + x[1] * x[1] + x[2] * x[2];

      // Записываем в x[] вектор от p_i до ближайшей точки,
      // а в x2 -- квадрат его длины.
      if (xr + r2 < 0.0)
      {
        x2 += r2 + 2 * xr;
        x[0] += r[0];
        x[1] += r[1];
        x[2] += r[2];
      }
      else if (xr < 0.0)
      {
        tau = xr / r2;
        x2 -= tau * xr;
        x[0] -= tau * r[0];
        x[1] -= tau * r[1];
        x[2] -= tau * r[2];
      }

      // Добавляем к градиенту в p_i слагаемое от взаимодействия с p_jp_{j+1}.
      for (k = 0; k < 3; k++)
        delta[i][k] -= len_table[j] / x2 * (x[k] * lt / x2 + local[k]);
    }
  }

  // Находим максимум из длин (компонент) градиентов.
  double max_shift = 0.0;
  for (i = 0; i < this->points.size(); i++)
    for (k = 0; k < 3; k++)
      if (max_shift < fabs (delta[i][k]))
        max_shift = fabs (delta[i][k]);

  // Вычисляем коэффициент, на который нужно домножить градиент.
  double coeff = oldLen * oldLen / this->points.size() / this->points.size() / 10.0;
  if (coeff > oldLen / this->points.size() / max_shift / 5.0)
    coeff = oldLen / this->points.size() / max_shift / 5.0;

  // Делаем сдвиг в направлении градиента.
  for (i = 0; i < this->points.size(); i++) {
    points[i].x += delta[i][0] * coeff;
    points[i].y += delta[i][1] * coeff;
    points[i].z += delta[i][2] * coeff;
	}

  // Удаляем векторы градиента.
  for (i = 0; i < this->points.size(); i++)
    delete[] delta[i];
  delete[] delta;

  // Ломаная изменилась, так что все параметры придется пересчитывать.
  // Удаляем старые значения.
  clear_depend();

  // Восстанавливаем старую длину.
  setLength(oldLen);
}

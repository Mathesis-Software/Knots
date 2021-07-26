#include <math.h>
#include "knot.h"

void knot::center (void)
{
  // Узел перемещается так, чтобы его центр масс оказался
  // в начале координат.

  double deltaX = 0.0,
         deltaY = 0.0,
         deltaZ = 0.0;

  for (int i = 0; i < length; i++)
  {
    deltaX += points [i] [0];
    deltaY += points [i] [1];
    deltaZ += points [i] [2];
  }

  deltaX /= length;
  deltaY /= length;
  deltaZ /= length;

  for (int i = 0; i < length; i++)
  {
    points [i] [0] -= deltaX;
    points [i] [1] -= deltaY;
    points [i] [2] -= deltaZ;
  }
}

void knot::setLength (double len)
{
  // Длина ломаной устанавливается равной len.

  len /= Length -> value ();

  for (int i = 0; i < length; i++)
    for (int k = 0; k < 3; k++)
      points [i] [k] *= len;

  // Все параметры придется пересчитывать.
  clear_depend ();
}

void knot::decreaseEnergy (void)
{
  // Сохраняем длину кривой, чтобы в конце восстановить ее.
  double oldLen = Length -> value ();

  // Расставляем точки на кривой равномерно.
  normalize (length);

  // Создаем массив расстояний между соседними точками. 
  create_len_table ();

  int i, j, k;
  double xr, x2, r2, tau, lt, x [3], r [3], local [3];
  double **delta = new double* [length];

  // Вычисляем вектор градиента в каждой вершине ломаной.
  for (i = 0; i < length; i++)
  {
    // Создаем и обнуляем вектор градиента для p_i.
    delta [i] = new double [3];
    for (k = 0; k < 3; k++)
      delta [i] [k] = 0.0;
    // Вычисляем общие коэффициенты для всех слагаемых в p_i.
    lt = len_table [i] + len_table [prev (i)];
    for (k = 0; k < 3; k++)
      local [k] =
        (points [i] [k] - points [next (i)] [k]) / len_table [i] +
        (points [i] [k] - points [prev (i)] [k]) / len_table [prev (i)]; 

    for (j = next (i); j != prev (i); j = next (j))
    {
      // Ищем ближайшую к p_i точку на ребре p_jp_{j+1}:
      //   если -xr / r2 < 0, это p_j,
      //   если -xr / r2 > 1 -- p_{j+1},
      //   иначе -- точка внутри ребра.
      x [0] = points [j] [0] - points [i] [0];
      x [1] = points [j] [1] - points [i] [1];
      x [2] = points [j] [2] - points [i] [2];
      r [0] = points [next (j)] [0] - points [j] [0];
      r [1] = points [next (j)] [1] - points [j] [1];
      r [2] = points [next (j)] [2] - points [j] [2];
      xr = x [0] * r [0] + x [1] * r [1] + x [2] * r [2];
      r2 = len_table [j] * len_table [j];
      x2 = x [0] * x [0] + x [1] * x [1] + x [2] * x [2];

      // Записываем в x[] вектор от p_i до ближайшей точки,
      // а в x2 -- квадрат его длины.
      if (xr + r2 < 0.0)
      {
        x2 += r2 + 2 * xr;
        x [0] += r [0];
        x [1] += r [1];
        x [2] += r [2];
      }
      else if (xr < 0.0)
      {
        tau = xr / r2;
        x2 -= tau * xr;
        x [0] -= tau * r [0];
        x [1] -= tau * r [1];
        x [2] -= tau * r [2];
      }

      // Добавляем к градиенту в p_i слагаемое от взаимодействия с p_jp_{j+1}.
      for (k = 0; k < 3; k++)
        delta [i] [k] -= len_table [j] / x2 * (x [k] * lt / x2 + local [k]); 
    }
  }

  // Находим максимум из длин (компонент) градиентов. 
  double max_shift = 0.0;
  for (i = 0; i < length; i++)
    for (k = 0; k < 3; k++)
      if (max_shift < fabs (delta [i] [k]))
        max_shift = fabs (delta [i] [k]);

  // Вычисляем коэффициент, на который нужно домножить градиент.
  double coeff = oldLen * oldLen / length / length / 10.0;
  if (coeff > oldLen / length / max_shift / 5.0)
    coeff = oldLen / length / max_shift / 5.0;

  // Делаем сдвиг в направлении градиента.
  for (i = 0; i < length; i++)
    for (k = 0; k < 3; k++)
      points [i] [k] += delta [i] [k] * coeff;

  // Удаляем векторы градиента.
  for (i = 0; i < length; i++)
    delete[] delta [i];
  delete[] delta;

  // Ломаная изменилась, так что все параметры придется пересчитывать.
  // Удаляем старые значения.
  clear_depend ();

  // Восстанавливаем старую длину.
  setLength (oldLen);
}

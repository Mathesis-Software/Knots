// #include <iostream.h>
#include <math.h>

#include "seifert.h"

const int MAX_ITERATION_NUMBER = 20; // Максимальное число итераций при
                                     // добавлении новой точки в заданном
				     // направлении.
const double MIN_TAU = 0.0001;       // Итерации при добавлении точки в
                                     // заданном направлении прекращаются,
                                     // если скалярное произведение вектора
				     // сдвига и суммы градиентов на его
				     // концах не превосходит этого числа.
const int MAX_POINTS_NUMBER = 8000;  // Максимальное число точек поверхности.
const double MIN_EPS = 0.024;        // Минимальное и максимальное значения
const double MAX_EPS = 0.04;         // допустимого сдвига.
const double NEAR = 1.5;             // Соседними считаются точки, находящиеся
                                     // друг от друга
				     // на расстоянии <= NEAR * Eps.
const double TIMES = 2.0;            // Значение допустимого сдвига не
                                     // превосходит расстояния до узла,
				     // деленного на TIMES.


static int counter = 0;

void seifert::addPoint (double x_dir, double y_dir, double z_dir)
{
  // Создаем новую точку в заданном направлении от текущей
  // на расстоянии localEps.

  double appr [3], gradient2 [3], eps, tau = 1.0;

  // Делаем несколько последовательных приближений.
  for (int i = 0; (fabs (tau) > MIN_TAU) && (i < MAX_ITERATION_NUMBER); i++)
  {
    // Считаем приближение для новой точки.
    eps = sqrt (x_dir * x_dir +
                y_dir * y_dir +
                z_dir * z_dir) / localEps;

    appr [0] = coord [0] + x_dir / eps;
    appr [1] = coord [1] + y_dir / eps;
    appr [2] = coord [2] + z_dir / eps;

    // Считаем сумму градиентов в начальной точке и
    // в приближении новой точки.
    this->base.getGradient (appr, gradient2);

    gradient2 [0] += gradient [0];
    gradient2 [1] += gradient [1];
    gradient2 [2] += gradient [2];

    // Подправляем направление, чтобы оно стало перпендикулярно
    // сумме градиентов.
    tau = (x_dir * gradient2 [0] +
           y_dir * gradient2 [1] +
           z_dir * gradient2 [2]) /
          (gradient2 [0] * gradient2 [0] +
           gradient2 [1] * gradient2 [1] +
           gradient2 [2] * gradient2 [2]);
    x_dir -= tau * gradient2 [0];
    y_dir -= tau * gradient2 [1];
    z_dir -= tau * gradient2 [2];
  }

  // Считаем последнее приближение и добавляем точку.
  eps = sqrt (x_dir * x_dir +
              y_dir * y_dir +
              z_dir * z_dir) / localEps;
  new seifert (coord [0] + x_dir / eps,
               coord [1] + y_dir / eps,
	       coord [2] + z_dir / eps, base, this);
}

void seifert::addPoint60 (double x_dir, double y_dir, double z_dir)
{
  // Создаем новую точку в направлении, составляющем 60^o
  // с направлением dir, перпендикулярном градиенту в данной точке
  // и составляющем с этими направлениями тройку фиксированного знака.
  // (Предполагается, что данное направление почти перпендикулярно
  // градиенту.)
  double x_orth2 = y_dir * gradient [2] - z_dir * gradient [1],
         y_orth2 = z_dir * gradient [0] - x_dir * gradient [2],
         z_orth2 = x_dir * gradient [1] - y_dir * gradient [0],
         len_dir = sqrt (x_dir * x_dir + y_dir * y_dir + z_dir * z_dir),
         len_orth2 = sqrt ( (x_orth2 * x_orth2 +
	                     y_orth2 * y_orth2 +
	                     z_orth2 * z_orth2) / 3);
  addPoint (x_dir / len_dir + x_orth2 / len_orth2,
            y_dir / len_dir + y_orth2 / len_orth2,
	    z_dir / len_dir + z_orth2 / len_orth2);
}

void seifert::searchForNeighbor (void)
{
  // Здесь ищутся соседи среди уже существующих точек
  // (просто просматривается весь список).

  seifert_ord *o;

  for (o = sord -> next;
       o && (o -> value -> coord [0] - coord [0] < NEAR * localEps);
       o = o -> next)
  {
    double coor [3], grad [3];
    for (int i = 0; i < 3; i++)
    {
      coor [i] = coord [i] - o -> value -> coord [i];
      grad [i] = gradient [i] + o -> value -> gradient [i];
    }
    double dist2 = coor [0] * coor [0] +
                   coor [1] * coor [1] +
                   coor [2] * coor [2];
    double grad2 = grad [0] * grad [0] +
                   grad [1] * grad [1] +
                   grad [2] * grad [2];
    double scal  = coor [0] * grad [0] +
                   coor [1] * grad [1] +
                   coor [2] * grad [2];

    // Проверяем расстояние и ``почти перпендикулярность'' градиенту,
    // добавляем точки в окрестности друг друга, если нужно.
    if ( (dist2 < NEAR * NEAR * localEps * localEps) && (scal * scal < 0.1 * dist2 * grad2) )
    {
      neighborhood -> insert (o -> value);
      o -> value -> neighborhood -> insert (this);
    }
  }

  for (o = sord -> prev;
       o && (coord [0] - o -> value -> coord [0] < NEAR * localEps);
       o = o -> prev)
  {
    double coor [3], grad [3];
    for (int i = 0; i < 3; i++)
    {
      coor [i] = coord [i] - o -> value -> coord [i];
      grad [i] = gradient [i] + o -> value -> gradient [i];
    }
    double dist2 = coor [0] * coor [0] +
                   coor [1] * coor [1] +
                   coor [2] * coor [2];
    double grad2 = grad [0] * grad [0] +
                   grad [1] * grad [1] +
                   grad [2] * grad [2];
    double scal  = coor [0] * grad [0] +
                   coor [1] * grad [1] +
                   coor [2] * grad [2];

    // Проверяем расстояние и ``почти перпендикулярность'' градиенту,
    // добавляем точки в окрестности друг друга, если нужно.
    if ( (dist2 < NEAR * NEAR * localEps * localEps) && (scal * scal < 0.1 * dist2 * grad2) )
    {
      neighborhood -> insert (o -> value);
      o -> value -> neighborhood -> insert (this);
    }
  }
}

void seifert::checkNeighborhood (void)
{
  // Проверяем, не нужно ли здесь остановиться по внешним причинам.
  // (Выход за границы области видимости, добрались до узла и т.п.)
  if (this->base.noMorePoints (coord))
    return;

  // Если соседей вообще нет (т.е. это просто первая точка),
  // то создаем первую точку в направлении, перпендикулярном градиенту.
  if (!neighborhood -> value)
  {
    if ( (fabs (gradient [0]) > fabs (gradient [1])) &&
         (fabs (gradient [0]) > fabs (gradient [2])) )
      addPoint (- gradient [1], gradient [0], 0);
    else
      addPoint (0, - gradient [2], gradient [1]);
  }

  // Если сосед ровно один, создаем второго соседа, чтобы
  // точка и 2 ее соседа образовывали равносторонний треугольник.
  if (neighborhood -> next == neighborhood)
    addPoint60 (neighborhood -> value -> coord [0] - coord [0],
                neighborhood -> value -> coord [1] - coord [1],
                neighborhood -> value -> coord [2] - coord [2]);

  // Окрестность проверяется на предмет наличия ``зазоров''
  // (углов > 90^o между направлениями на соседей), если таковые есть,
  // то они заполняются (вставляется новая точка на расстоянии localEps от
  // данной, чтобы угол между направлениями на новую точку и на первого
  // из соседей был равен 60^o.
  {
    // Это векторы направления на двух соседей.
    double vect1 [3], vect2 [3];

    seifert_list *start = neighborhood;
    for (;;)
    {
      for (int i = 0; i < 3; i++)
      {
        vect1 [i] = neighborhood -> value -> coord [i] - coord [i]; 
        vect2 [i] = neighborhood -> next -> value -> coord [i] - coord [i]; 
      }

      if ( (vect1 [0] * vect2 [0] +
            vect1 [1] * vect2 [1] +
  	    vect1 [2] * vect2 [2] < 0) ||
           (gradient [0] *
	      (vect1 [2] * vect2 [1] - vect1 [1] * vect2 [2]) + 
            gradient [1] *
	      (vect1 [0] * vect2 [2] - vect1 [2] * vect2 [0]) + 
            gradient [2] *
	      (vect1 [1] * vect2 [0] - vect1 [0] * vect2 [1]) < - 0.001 * MAX_EPS * MAX_EPS) )
        addPoint60 (vect1 [0], vect1 [1], vect1 [2]);

      if (counter > MAX_POINTS_NUMBER - 1)
        break;
      neighborhood = neighborhood -> next;
      if (neighborhood == start)
        break;
    }
  }
}

seifert_list *seifert::hasNeighbor (seifert *n)
{
  for (seifert_list *sl = neighborhood; ; sl = sl -> next)
  {
    if (sl -> value == n)
      return sl;
    if (sl -> next == neighborhood)
      return NULL;
  }
}

void seifert::markUsed (seifert *start, seifert *end)
{
  seifert_list *sl_start = hasNeighbor (start);
  if (!sl_start)
    return;

  seifert_list *sl_end = hasNeighbor (end);
  if (!sl_end)
    return;

  for (; sl_start != sl_end; sl_start = sl_start -> next)
    sl_start -> label = 1;
}

void seifert::correction_local (void)
{
  if (!neighborhood -> value)
    return;

  for (seifert_list *sl = neighborhood; ; sl = sl -> next)
  {
    sl -> value -> neighborhood -> insert (sl -> next -> value);
    sl -> next -> value -> neighborhood -> insert (sl -> value);
    if (sl -> next == neighborhood)
      break;
  }
}

void seifert::correction (void)
{
  correction_local ();

  seifert_ord *so = sord;
  while (so -> prev)
  {
    so = so -> prev;
    so -> value -> correction_local ();
  }

  so = sord;
  while (so -> next)
  {
    so = so -> next;
    so -> value -> correction_local ();
  }
}

seifert::seifert (const double x, const double y, const double z,
                  const seifert_base &base, seifert* neighbor) : base(base) {
  counter ++;
//  cerr << "Point " << counter << '\n';

  coord [0] = x;
  coord [1] = y;
  coord [2] = z;

	this->base.getGradient (coord, gradient);

	localEps = this->base.minDist (coord) / TIMES;
	if (localEps > MAX_EPS)
		localEps = MAX_EPS;

	if (neighbor)
		sord = neighbor -> sord -> insert (this);
	else
		sord = new seifert_ord(this);

  neighborhood = new seifert_list(this, neighbor);
  if (neighbor) {
    neighbor -> neighborhood -> insert_after (this);
    searchForNeighbor ();
  }

  if (localEps > MIN_EPS) {
    checkNeighborhood();
	}
}

seifert::~seifert (void)
{
  if (sord -> next)
  {
    sord -> next -> prev = NULL;
    delete sord -> next -> value;
    sord -> next = NULL;
  }
  if (sord -> prev)
  {
    sord -> prev -> next = NULL;
    delete sord -> prev -> value;
    sord -> prev = NULL;
  }
  delete sord;
  delete neighborhood;

  counter --;
}

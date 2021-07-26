#include <math.h>
#include "seifert.h"

static bool test (const double *v0,
                  const double *v1,
		  const double *v2,
		  const double *v3)
{
  // Вспомогательная процедура, проверяющая циклический порядок векторов
  // v1, v2, v3. Вектор v0 ортогонален им всем и задает циклический порядок.
  double u1 [3], u2 [3];
  u1 [0] = v1 [0] - v2 [0];
  u1 [1] = v1 [1] - v2 [1];
  u1 [2] = v1 [2] - v2 [2];
  u2 [0] = v1 [0] - v3 [0];
  u2 [1] = v1 [1] - v3 [1];
  u2 [2] = v1 [2] - v3 [2];
  return (v0 [0] * (u1 [1] * u2 [2] - u1 [2] * u2 [1]) +
          v0 [1] * (u1 [2] * u2 [0] - u1 [0] * u2 [2]) +
	  v0 [2] * (u1 [0] * u2 [1] - u1 [1] * u2 [0]) > 0);
}

static double *orthn (const double *v0, double *v1)
{
  // Вспомогательная процедура. Делает вектор v1 ортогональным v0
  // и нормирует его.
  double a = (v0 [0] * v1 [0] + v0 [1] * v1 [1] + v0 [2] * v1 [2]) /
             (v0 [0] * v0 [0] + v0 [1] * v0 [1] + v0 [2] * v0 [2]);
  v1 [0] -= a * v0 [0];
  v1 [1] -= a * v0 [1];
  v1 [2] -= a * v0 [2];
    
  double len = sqrt (v1 [0] * v1 [0] + v1 [1] * v1 [1] + v1 [2] * v1 [2]);
  v1 [0] /= len;
  v1 [1] /= len;
  v1 [2] /= len;

  return v1;
}

void seifert_list::insert_after (seifert *s)
{
  // Процедура добавления нового соседа сразу после первого,
  // ничего не проверяя.

  // Это на случай, если раньше соседей вообще не было.
  if (value == NULL)
  {
    value = s;
    return;
  }

  seifert_list *sl = new seifert_list (basepoint, s);
  sl -> prev = this;
  sl -> next = next;
  next -> prev = sl;
  next = sl;
}

void seifert_list::insert (seifert *s)
{
  // Процедура добавления нового соседа с поиском правильного места для него.
  
  // Если в окрестности не было ничего, все просто.
  if (value == NULL)
  {
    value = s;
    return;
  }

  // Проверка, нет ли уже этого соседа.
  {
    seifert_list *sl = this;
    for (;;)
    {
      if (sl -> value == s)
        return;
      sl = sl -> next;
      if (sl == this)
        break;
    }
  }

  // Если был всего один сосед -- тоже не хитро.
  if (next == this)
  {
    next = new seifert_list (basepoint, s);
    prev = next;
    next -> prev = this;
    next -> next = this;
    return;
  }

  {
    int i;

    // Создаем переменные для векторов направлений из центра на соседей,
    // vect0 -- для нового соседа, vect1 и vect2 -- для двух подряд
    // идущих старых. Записываем в vect0 и vect1 начальные значения.
    double vect0 [3],
         *vect1 = new double [3],
	 *vect2 = new double [3],
	 *vect_tmp;
    for (i = 0; i < 3; i++)
    {
      vect0 [i] = s -> coord [i] - basepoint -> coord [i];
      vect1 [i] = value -> coord [i] - basepoint -> coord [i];
    }
    orthn (basepoint -> gradient, vect0);
    orthn (basepoint -> gradient, vect1);

    // Теперь цикл для поиска места, куда вставить нового соседа.
    seifert_list *current = this;
    for (;;)
    {
      // Создаем и нормируем vect2.
      for (i = 0; i < 3; i++)
        vect2 [i] = current -> next -> value -> coord [i] - basepoint -> coord [i];
      orthn (basepoint -> gradient, vect2);

      // Проверяем, сюда ли следует вставить нового соседа.
      if ( test (basepoint -> gradient, vect0, vect1, vect2) )
        break;

      // Не получилось -- переходим к следующей паре векторов.
      // Останавливаемся, если дальше некуда идти.
      current = current -> next;
      if (current -> next == this)
        break;

      // Меняем местами указатели на vect1 и vect2 -- значения в vect1
      // нам еще пригодятся, а значения в vect2 придется считать заново.
      vect_tmp = vect1;
      vect1 = vect2;
      vect2 = vect_tmp;
    }
    
    delete[] vect1;
    delete[] vect2;

    // Вставляем, наконец, нового соседа.
    seifert_list *sl = new seifert_list (basepoint, s);
    sl -> prev = current;
    sl -> next = current -> next;
    current -> next -> prev = sl;
    current -> next = sl;
  }
}

seifert_list::seifert_list (seifert *bp, seifert *s)
{
  basepoint = bp;
  value = s;
  next = this;
  prev = this;
  label = 0;
}

seifert_list::~seifert_list (void)
{
  for (seifert_list *current = next; next != this; current = next)
  {
    next = current -> next;
    current -> next = current;
    delete current;
  }
}

#include "seifert.h"

namespace {

double det(const double *v0, const double *v1, const double *v2, const double *orth) {
  return
      (v1 [0] - v0 [0]) * (v2 [1] - v0 [1]) * orth [2]
    - (v1 [0] - v0 [0]) * (v2 [2] - v0 [2]) * orth [1]
    - (v1 [1] - v0 [1]) * (v2 [0] - v0 [0]) * orth [2]
    + (v1 [1] - v0 [1]) * (v2 [2] - v0 [2]) * orth [0]
    + (v1 [2] - v0 [2]) * (v2 [0] - v0 [0]) * orth [1]
    - (v1 [2] - v0 [2]) * (v2 [1] - v0 [1]) * orth [0];
}

}

namespace KE { namespace GL {

SeifertSurface::SeifertSurface(const seifert_base &base, double *sp) : base(base) {
  start_point = sp;
  stripped = 0;
  sides = Both;
}

void SeifertSurface::addTriangles(seifert *s) {
  // Это оч. убогая версия создания поверхности из графа.
  // Просто в каждой вершине для любых двух ``соседних соседей''
  // создается новый треугольник, треугольники НИКАК не связаны.
  // Единственное, что отсекается -- многократное покрытие одного
  // и того же куска поверхности.
  // ?? -- !!

  // И если это единственная вершина -- ничего не делаем.
  if (!s -> neighborhood -> value)
    return;

  seifert_list *sl = s -> neighborhood;
  for (;;)
  {
    // Не нужно добавлять треугольник, если в этом месте уже есть поверхность.
    if (sl -> label)
    {
      sl = sl -> next;
      if (sl == s -> neighborhood)
        break;
      continue;
    }

    // Помечаем в соседних вершинах, какие треугольники не потребуются.
    sl -> value -> markUsed (sl -> next -> value, s);
    sl -> next -> value -> markUsed (s, sl -> value);

    // Добавляем треугольник.
    addpoint (s -> coord [0],
              s -> coord [1],
	      s -> coord [2],
	      s -> gradient [0],
	      s -> gradient [1],
	      s -> gradient [2]);
    // Правильно ориентируем треугольник.
    if (det (s -> coord,
             sl -> value -> coord,
             sl -> next -> value -> coord,
	     s -> gradient) < 0)
    {
      addpoint (sl -> next -> value -> coord [0],
                sl -> next -> value -> coord [1],
                sl -> next -> value -> coord [2],
	        sl -> next -> value -> gradient [0],
	        sl -> next -> value -> gradient [1],
	        sl -> next -> value -> gradient [2]);
      addpoint (sl -> value -> coord [0],
                sl -> value -> coord [1],
	        sl -> value -> coord [2],
	        sl -> value -> gradient [0],
	        sl -> value -> gradient [1],
	        sl -> value -> gradient [2]);
    }
    else
    {
      addpoint (sl -> value -> coord [0],
                sl -> value -> coord [1],
	        sl -> value -> coord [2],
	        sl -> value -> gradient [0],
	        sl -> value -> gradient [1],
	        sl -> value -> gradient [2]);
      addpoint (sl -> next -> value -> coord [0],
                sl -> next -> value -> coord [1],
                sl -> next -> value -> coord [2],
	        sl -> next -> value -> gradient [0],
	        sl -> next -> value -> gradient [1],
	        sl -> next -> value -> gradient [2]);
    }

    // Следующий сосед.
    sl = sl -> next;
    if (sl == s -> neighborhood)
      break;
  }
}

void SeifertSurface::calculate() {
  // Создаем граф поверхности.
  seifert *s = new seifert (start_point [0],
                            start_point [1],
			    start_point [2],
			    base);
  s -> correction ();

  // Создаем поверхность.

  addTriangles (s);
  seifert_ord *so = s -> sord;
  while (so -> prev)
  {
    so = so -> prev;
    addTriangles (so -> value);
  }
  so = s -> sord;
  while (so -> next)
  {
    so = so -> next;
    addTriangles (so -> value);
  }

  // Удаляем граф.
  delete s;
}

}}

#include "seifert.h"

namespace KE { namespace GL {

namespace {

double det(const ThreeD::Point &pt0, const ThreeD::Point &pt1, const ThreeD::Point &pt2, const ThreeD::Vector &orth) {
  return
      (pt1.x - pt0.x) * (pt2.y - pt0.y) * orth.z
    - (pt1.x - pt0.x) * (pt2.z - pt0.z) * orth.y
    - (pt1.y - pt0.y) * (pt2.x - pt0.x) * orth.z
    + (pt1.y - pt0.y) * (pt2.z - pt0.z) * orth.x
    + (pt1.z - pt0.z) * (pt2.x - pt0.x) * orth.y
    - (pt1.z - pt0.z) * (pt2.y - pt0.y) * orth.x;
}

}

SeifertSurface::SeifertSurface(const ThreeD::Knot &base, const ThreeD::Point &startPoint) : base(base), startPoint(startPoint) {
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
  if (!s->neighborhood->value)
    return;

  seifert_list *sl = s->neighborhood;
  for (;;)
  {
    // Не нужно добавлять треугольник, если в этом месте уже есть поверхность.
    if (sl->label)
    {
      sl = sl->next;
      if (sl == s->neighborhood)
        break;
      continue;
    }

    // Помечаем в соседних вершинах, какие треугольники не потребуются.
    sl->value->markUsed (sl->next->value, s);
    sl->next->value->markUsed (s, sl->value);

    // Добавляем треугольник.
    addpoint(s->point, s->gradient);
    // Правильно ориентируем треугольник.
    if (det(s->point, sl->value->point, sl->next->value->point, s->gradient) < 0) {
      addpoint(sl->next->value->point, sl->next->value->gradient);
      addpoint(sl->value->point, sl->value->gradient);
    } else {
      addpoint(sl->value->point, sl->value->gradient);
      addpoint(sl->next->value->point, sl->next->value->gradient);
    }

    // Следующий сосед.
    sl = sl->next;
    if (sl == s->neighborhood)
      break;
  }
}

void SeifertSurface::calculate() {
  // Создаем граф поверхности.
  seifert *s = new seifert(base, this->startPoint);
  s->correction();

  // Создаем поверхность.

  addTriangles (s);
  seifert_ord *so = s->sord;
  while (so->prev) {
    so = so->prev;
    addTriangles (so->value);
  }
  so = s->sord;
  while (so->next) {
    so = so->next;
    addTriangles (so->value);
  }

  // Удаляем граф.
  delete s;
}

}}

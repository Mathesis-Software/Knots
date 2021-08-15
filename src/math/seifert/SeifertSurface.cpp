#include "seifert.h"
#include "SeifertSurface.h"

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

ThreeD::Vector SeifertSurface::gradient(const ThreeD::Point &point, const ThreeD::Knot::Snapshot &snapshot) {
	ThreeD::Vector gradient(0.0, 0.0, 0.0);

	for (std::size_t i = 0; i < snapshot.size(); i++) {
		const ThreeD::Vector x(point, snapshot[i]);
		const ThreeD::Vector r(snapshot[i], snapshot[snapshot.next(i)]);

		const double xr = x.scalar_product(r);
		const double x2 = x.square();
		const double r2 = r.square();
		const double tau = - xr / r2;
		const double a2 = x2 + tau * xr;
		const double coeff = ((tau - 1) / sqrt (x2 + r2 + xr + xr) - tau / sqrt(x2)) / a2;

		gradient.x += (r.y * x.z - r.z * x.y) * coeff;
		gradient.y += (r.z * x.x - r.x * x.z) * coeff;
		gradient.z += (r.x * x.y - r.y * x.x) * coeff;
	}

	gradient.normalize();
	return gradient;
}

SeifertSurface::SeifertSurface(const ThreeD::KnotWrapper &base, const ThreeD::Point &startPoint) : Surface(false, true), base(base), startPoint(startPoint) {
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

bool SeifertSurface::destroy(bool force) {
	if (!force && this->stored && !this->stored->isObsolete()) {
		return false;
	}
	this->stored = std::make_shared<ThreeD::Knot::Snapshot>(this->base.snapshot());
	Surface::destroy();
	return true;
}

void SeifertSurface::calculate() {
  // Создаем граф поверхности.
  seifert *s = new seifert(this->base.snapshot(), this->startPoint);
  s->correction();

  // Создаем поверхность.

  addTriangles(s);
  seifert_ord *so = s->sord;
  while (so->prev) {
    so = so->prev;
    addTriangles(so->value);
  }
  so = s->sord;
  while (so->next) {
    so = so->next;
    addTriangles(so->value);
  }

  // Удаляем граф.
  delete s;
}

}}

/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at

 *   http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 * Author: Nikolay Pultsin <geometer@geometer.name>
 */

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

bool SeifertSurface::isVisible() const {
	const auto ref = this->base.isSeifertSurfaceVisible;
	return ref && *ref;
}

const Color &SeifertSurface::frontColor() const {
	const auto ref = this->base.seifertFrontColor;
	return ref ? *ref : Color::white;
}

const Color &SeifertSurface::backColor() const {
	const auto ref = this->base.seifertBackColor;
	return ref ? *ref : Color::white;
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

bool SeifertSurface::isObsolete() const {
	return Surface::isObsolete() || !this->stored || this->stored->isObsolete();
}

void SeifertSurface::calculate() {
	const auto snapshot = this->base.snapshot();
	this->stored = std::make_shared<ThreeD::Knot::Snapshot>(snapshot);

  // Создаем граф поверхности.
  seifert *s = new seifert(snapshot, this->startPoint);
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

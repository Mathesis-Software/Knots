/*
 * Copyright (c) 1995-2021, Nikolay Pultsin <geometer@geometer.name>
 *
 * Licensed under the Apache License, Version 2.0 the "License";
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <cmath>

#include "seifert.h"

namespace KE::GL {

namespace {

bool test(const ThreeD::Vector &v0, const ThreeD::Vector &v1, const ThreeD::Vector &v2, const ThreeD::Vector &v3) {
  // Вспомогательная процедура, проверяющая циклический порядок векторов
  // v1, v2, v3. Вектор v0 ортогонален им всем и задает циклический порядок.
	ThreeD::Vector u1(v1);
	u1.add(v2, -1);
	ThreeD::Vector u2(v1);
	u2.add(v3, -1);
	return v0.scalar_product(u1.vector_product(u2)) > 0;
}

void orthn(const ThreeD::Vector &v0, ThreeD::Vector &v1) {
  // Вспомогательная процедура. Делает вектор v1 ортогональным v0
  // и нормирует его.
	const double a = v0.scalar_product(v1) / v0.square();
	v1.add(v0, -a);
	v1.normalize();
}

}

void seifert_list::insert_after(seifert *s) {
  // Процедура добавления нового соседа сразу после первого,
  // ничего не проверяя.

  // Это на случай, если раньше соседей вообще не было.
  if (value == nullptr) {
    value = s;
    return;
  }

  seifert_list *sl = new seifert_list(basepoint, s);
  sl->prev = this;
  sl->next = next;
  next->prev = sl;
  next = sl;
}

void seifert_list::insert(seifert *s) {
  // Процедура добавления нового соседа с поиском правильного места для него.

  // Если в окрестности не было ничего, все просто.
  if (value == nullptr) {
    value = s;
    return;
  }

  // Проверка, нет ли уже этого соседа.
  {
    seifert_list *sl = this;
    for (;;) {
      if (sl->value == s)
        return;
      sl = sl->next;
      if (sl == this)
        break;
    }
  }

  // Если был всего один сосед -- тоже не хитро.
  if (next == this) {
    next = new seifert_list(basepoint, s);
    prev = next;
    next->prev = this;
    next->next = this;
    return;
  }

  {
    // Создаем переменные для векторов направлений из центра на соседей,
    // vect0 -- для нового соседа, vect1 и vect2 -- для двух подряд
    // идущих старых. Записываем в vect0 и vect1 начальные значения.
		ThreeD::Vector vect0(basepoint->point, s->point);
		ThreeD::Vector vect1(basepoint->point, value->point);
    orthn(basepoint->gradient, vect0);
    orthn(basepoint->gradient, vect1);

    // Теперь цикл для поиска места, куда вставить нового соседа.
    seifert_list *current = this;
    for (;;) {
      // Создаем и нормируем vect2.
			ThreeD::Vector vect2(basepoint->point, current->next->value->point);
      orthn(basepoint->gradient, vect2);

      // Проверяем, сюда ли следует вставить нового соседа.
      if (test(basepoint->gradient, vect0, vect1, vect2)) {
        break;
			}

      // Не получилось -- переходим к следующей паре векторов.
      // Останавливаемся, если дальше некуда идти.
      current = current->next;
      if (current->next == this)
        break;

      // Меняем местами vect1 и vect2 -- значения в vect1
      // нам еще пригодятся, а значения в vect2 придется считать заново.
			vect1.swap(vect2);
    }

    // Вставляем, наконец, нового соседа.
    seifert_list *sl = new seifert_list(basepoint, s);
    sl->prev = current;
    sl->next = current->next;
    current->next->prev = sl;
    current->next = sl;
  }
}

seifert_list::seifert_list(seifert *bp, seifert *s) {
  basepoint = bp;
  value = s;
  next = this;
  prev = this;
  label = 0;
}

seifert_list::~seifert_list() {
  for (seifert_list *current = next; next != this; current = next) {
    next = current->next;
    current->next = current;
    delete current;
  }
}

}

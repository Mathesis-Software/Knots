/*
 * Copyright (c) 1995-2021, Mathesis Software <mad@mathesis.fun>
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

#include "seifert.h"

namespace KE::GL {

seifert_ord *seifert_ord::insert(seifert *s) {
  if (value->point.x > s->point.x) {
    if (!prev) {
      prev = new seifert_ord (s);
      prev->next = this;
      return prev;
    } else {
      return prev->insert (s);
		}
  } else if (!next) {
		next = new seifert_ord (s);
		next->prev = this;
		return next;
	} else if (next->value->point.x > s->point.x) {
		seifert_ord *so = new seifert_ord (s);
		so->next = next;
		so->prev = this;
		next->prev = so;
		next = so;
		return so;
	} else {
		return next->insert (s);
  }
}

seifert_ord::seifert_ord(seifert *s) {
  value = s;
  next = nullptr;
  prev = nullptr;
}

seifert_ord::~seifert_ord() {
  if (next)
    next->prev = nullptr;

  if (prev)
    prev->next = nullptr;
}

}

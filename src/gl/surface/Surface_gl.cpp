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

#include <GL/gl.h>

#include "Surface.h"

void KE::GL::Surface::paint() {
	// Поверхность перерисовывается, если она видима.
	if (!this->isVisible()) {
		return;
	}

	// Если нужно, сначала делаем вычисления.
	if (this->isObsolete()) {
		this->points.clear();
	}
	if (this->points.empty()) {
		this->calculate();
	}

	// Устанавливаем цвета обеих сторон поверхности.
	glMaterialfv(GL_FRONT, GL_DIFFUSE, this->frontColor().rgb);
	glMaterialfv(GL_BACK, GL_DIFFUSE, this->backColor().rgb);

	// Двусторонняя ли поверхность?
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, this->showBackSide);

	// Связаны ли треугольники?
	glBegin(stripped ? GL_TRIANGLE_STRIP : GL_TRIANGLES);

	for (const auto &pt : this->points) {
		glNormal3fv(pt.normal);
		glVertex3fv(pt.vertex);
	}

	glEnd();
}

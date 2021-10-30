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

#include <QtGui/QMouseEvent>

#include "GLWidget.h"

namespace KE::Qt {

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent), scaleFactor(1.0f) {
	for (int i = 0; i < 16; ++i) {
		this->_currentMatrix[i] = (i % 5) ? 0.0 : 1.0;
		this->_inverseMatrix[i] = (i % 5) ? 0.0 : 1.0;
	}
	auto format = this->format();
	format.setSamples(4);
	this->setFormat(format);
}

void GLWidget::resizeGL(int w, int h) {
	glViewport(0, 0, w, h);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-w / 800.0, w / 800.0, -h / 800.0, h / 800.0, 5, 25);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -10.0);
	glMultMatrixd(this->currentMatrix());
}

void GLWidget::initializeGL() {
	float pos[] = {5.0, 5.0, 10.0, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	glEnable(GL_RESCALE_NORMAL);
}

void GLWidget::paintGL() {
	const auto bg = &this->backgroundColor();
	glClearColor(bg->rgb[0], bg->rgb[1], bg->rgb[2], 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (const auto &surface : this->surfaces) {
		this->paintSurface(*surface);
	}

	glFlush();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
	this->capturedPoint = event->pos();
	this->selectMouseCursor();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
	if (!this->capturedPoint.isNull()) {
		this->rotate(this->capturedPoint, event->pos(), event->modifiers());
		this->capturedPoint = event->pos();
	}
}

static const float SCALE_STEP = 1.05f;

void GLWidget::wheelEvent(QWheelEvent *event)  {
	auto delta = event->angleDelta();

	if (delta.y() < 0)  {
		scaleFactor /= SCALE_STEP;
	} else if (delta.y() > 0)  {
		scaleFactor *= SCALE_STEP;
	}

	this->update();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
	if (!this->capturedPoint.isNull()) {
		this->rotate(this->capturedPoint, event->pos(), event->modifiers());
	}
	this->capturedPoint = QPoint();
	this->selectMouseCursor();
}

void GLWidget::selectMouseCursor() {
	if (!this->capturedPoint.isNull()) {
		this->setCursor(::Qt::DragMoveCursor);
	} else {
		this->unsetCursor();
	}
}

const double *GLWidget::currentMatrix() const {
	this->prepareMatrix(this->_currentMatrix, false);
	return this->_currentMatrix;
}

const double *GLWidget::inverseMatrix() const {
	this->prepareMatrix(this->_inverseMatrix, true);
	return this->_inverseMatrix;
}

void GLWidget::rotate(const QPoint &start, const QPoint &end, ::Qt::KeyboardModifiers modifiers) {
	this->makeCurrent();
	glMultMatrixd(this->inverseMatrix());

	double dx = end.x() - start.x();
	double dy = end.y() - start.y();
	double dz = 0;

	if (modifiers & ::Qt::AltModifier) {
		const int centerX = this->width() / 2;
		const int centerY = this->height() / 2;
		dx = 0;
		dy = 0;
		dz = ((end.x() - centerX) * (start.y() - centerY) - (start.x() - centerX) * (end.y() - centerY)) / hypot(centerX, centerY);
	} else if (modifiers & ::Qt::ShiftModifier) {
		if (abs(dx) > abs(dy)) {
			dy = 0;
		} else {
			dx = 0;
		}
	}

	const double step = 0.006;
	this->rotate(step * dx, step * dy, step * dz);

	glMultMatrixd(this->currentMatrix());
	this->update();
}

void GLWidget::paintSurface(const GL::Surface &surface) {
	// Поверхность перерисовывается, если она видима.
	if (!surface.isVisible()) {
		return;
	}

	// Если нужно, сначала делаем вычисления.
	surface.prepare();

	// Устанавливаем цвета обеих сторон поверхности.
	glMaterialfv(GL_FRONT, GL_DIFFUSE, surface.frontColor().rgb);
	glMaterialfv(GL_BACK, GL_DIFFUSE, surface.backColor().rgb);

	// Двусторонняя ли поверхность?
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, surface.showBackSide);

	glScalef(scaleFactor, scaleFactor, scaleFactor);
	// Связаны ли треугольники?
	glBegin(surface.stripped ? GL_TRIANGLE_STRIP : GL_TRIANGLES);

	for (const auto &pt : surface.points()) {
		glNormal3fv(pt.normal);
		glVertex3fv(pt.vertex);
	}

	glEnd();

	glScalef(1.0 / scaleFactor, 1.0 / scaleFactor, 1.0 / scaleFactor);
}

}

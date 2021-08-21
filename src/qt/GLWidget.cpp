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

#include <QtGui/QMouseEvent>

#include "GLWidget.h"

namespace KE { namespace Qt {

GLWidget::GLWidget(QWidget *parent) : QOpenGLWidget(parent), currentMatrix(new double[16]) {
	for (int i = 0; i < 16; ++i) {
		this->currentMatrix[i] = (i % 5) ? 0.0 : 1.0;
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
	glMultMatrixd(this->currentMatrix.get());
}

void GLWidget::initializeGL() {
	float pos[] = {5.0, 5.0, 10.0, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
}

void GLWidget::paintGL() {
	const auto bg = &this->backgroundColor();
	glClearColor(bg->rgb[0], bg->rgb[1], bg->rgb[2], 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto surface : this->surfaces) {
		surface->paint();
	}

	glFlush();
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
	this->capturedPoint = event->pos();
	this->selectMouseCursor();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
	if (!this->capturedPoint.isNull()) {
		this->rotate(this->capturedPoint, event->pos());
		this->capturedPoint = event->pos();
	}
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
	if (!this->capturedPoint.isNull()) {
		this->rotate(this->capturedPoint, event->pos());
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

}}

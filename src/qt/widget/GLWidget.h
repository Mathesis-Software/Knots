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

#ifndef __GLWIDGET_H__
#define __GLWIDGET_H__

#include <list>
#include <memory>

#include <QtWidgets/QOpenGLWidget>

#include "../../gl/surface/Surface.h"

class GLWidget : public QOpenGLWidget {

private:
	std::list<std::shared_ptr<KE::GL::Surface>> surfaces;

	std::unique_ptr<double[]> currentMatrix;

	QPoint capturedPoint;

private:
	void initializeGL() override;
	void resizeGL(int, int) override;
	void paintGL();

	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;

	void rotate(const QPoint &start, const QPoint &end);
	void selectMouseCursor();

public:
	GLWidget(QWidget *parent);
	virtual const KE::GL::Color &backgroundColor() const = 0;

	void addSurface(std::shared_ptr<KE::GL::Surface> surface) { this->surfaces.push_back(surface); };
	void rotate(int, int);
	double currMatr(int i, int j) const { return this->currentMatrix[4 * i + j]; }
};

#endif /* __GLWIDGET_H__ */

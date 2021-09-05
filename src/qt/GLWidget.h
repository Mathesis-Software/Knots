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

#ifndef __GLWIDGET_H__
#define __GLWIDGET_H__

#include <list>
#include <memory>

#include <QtOpenGLWidgets/QOpenGLWidget>

#include "../ke/Surface.h"

namespace KE::Qt {

class GLWidget : public QOpenGLWidget {

private:
	std::list<std::shared_ptr<const GL::Surface>> surfaces;

	mutable double _currentMatrix[16];
	mutable double _inverseMatrix[16];

	QPoint capturedPoint;

public:
	GLWidget(QWidget *parent);
	virtual const Color &backgroundColor() const = 0;

	void addSurface(std::shared_ptr<const GL::Surface> surface) { this->surfaces.push_back(surface); };

protected:
	virtual void prepareMatrix(double *matrix, bool inverse) const = 0;
	virtual void rotate(double dx, double dy, double dz) = 0;

private:
	void initializeGL() override;
	void resizeGL(int, int) override;
	void paintGL() override;

	void mousePressEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;

	void rotate(const QPoint &start, const QPoint &end, ::Qt::KeyboardModifiers modifiers);
	void selectMouseCursor();

	void paintSurface(const GL::Surface &surface);

	const double* currentMatrix() const;
	const double* inverseMatrix() const;
};

}

#endif /* __GLWIDGET_H__ */

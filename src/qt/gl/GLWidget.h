#ifndef __GLWIDGET_H__
#define __GLWIDGET_H__

#include <list>
#include <memory>

#include <QtWidgets/QOpenGLWidget>

#include "../../gl/surface/surface.h"

class GLWidget : public QOpenGLWidget {

private:
	std::list<std::shared_ptr<KE::GL::Surface>> surfaces;

	float backgroundRGB[3];
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
	void addSurface(std::shared_ptr<KE::GL::Surface> surface) { this->surfaces.push_back(surface); };

	const float *getBackgroundRGB() const;
	void setBackgroundRGB(const float rgb[3]);

	void rotate(int, int);

	double currMatr(int i, int j) { return this->currentMatrix[4 * i + j]; }
};

#endif /* __GLWIDGET_H__ */

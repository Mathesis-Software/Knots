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

private:
	void initializeGL() override;
	void resizeGL(int, int) override;
	void paintGL();

public:
	GLWidget(QWidget *parent);
	void addSurface(std::shared_ptr<KE::GL::Surface> surface) { this->surfaces.push_back(surface); };

	const float *getBackgroundRGB() { return this->backgroundRGB; }
	void setBackgroundRGB(const float rgb[3]) {
		this->backgroundRGB[0] = rgb[0];
		this->backgroundRGB[1] = rgb[1];
		this->backgroundRGB[2] = rgb[2];
	}

	void rotate(int, int);

	double currMatr(int i, int j) { return this->currentMatrix[4 * i + j]; }
	void multMatrix() {
		makeCurrent();
		glMultMatrixd(this->currentMatrix.get());
	}
};

#endif /* __GLWIDGET_H__ */

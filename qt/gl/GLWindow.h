#ifndef __GLWINDOW_H__
#define __GLWINDOW_H__

#include <list>
#include <memory>

#include <QtWidgets/QOpenGLWidget>

#include "../abstractWindow/abstractWindow.h"
#include "../../gl/surface/surface.h"

class GLWindow : public abstractWindow {

private:
	std::list<std::shared_ptr<KE::GL::Surface>> surfaces;
	float backgroundRGB[3];

	std::unique_ptr<double[]> currentMatrix;
	std::unique_ptr<double[]> currentSpeedMatrix;

	bool isInertia;
	int timerId_rotate;

	void printIt(QPrinter*);

	void rotate(int, int);
	void changeSpeed(int, int);
	void doRotate();

	void inertia();
	void rotate(int);

protected:
	void timerEvent(QTimerEvent*);
	void addSurface(std::shared_ptr<KE::GL::Surface> surface) { this->surfaces.push_back(surface); };
	double currMatr(int i, int j) { return this->currentMatrix[4 * i + j]; }
	void repaint3d();

	const float *getBackgroundRGB() { return this->backgroundRGB; }
	void setBackgroundRGB(const float rgb[3]) {
		this->backgroundRGB[0] = rgb[0];
		this->backgroundRGB[1] = rgb[1];
		this->backgroundRGB[2] = rgb[2];
	}

public:
	GLWindow();

friend class GLWidget;
};

class GLWidget : public QOpenGLWidget {

private:
	GLWindow *Parent;
	void initializeGL() override;
	void resizeGL(int, int) override;
	void paintGL();

public:
	GLWidget(GLWindow*);

	void multMatrix() {
		makeCurrent();
		glMultMatrixd(Parent->currentMatrix.get());
	}
};

#endif /* __GLWINDOW_H__ */

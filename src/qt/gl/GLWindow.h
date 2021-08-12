#ifndef __GLWINDOW_H__
#define __GLWINDOW_H__

#include <list>
#include <memory>

#include <QtWidgets/QOpenGLWidget>

#include "../abstractWindow/abstractWindow.h"
#include "../../gl/surface/surface.h"

class GLWidget : public QOpenGLWidget {

friend class GLWindow;

private:
	std::list<std::shared_ptr<KE::GL::Surface>> surfaces;

	float backgroundRGB[3];
	std::unique_ptr<double[]> currentMatrix;
	std::unique_ptr<double[]> currentSpeedMatrix;

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

	double currMatr(int i, int j) { return this->currentMatrix[4 * i + j]; }
	void multMatrix() {
		makeCurrent();
		glMultMatrixd(this->currentMatrix.get());
	}
};

class GLWindow : public abstractWindow {

private:
	bool isInertia;
	int timerId_rotate;

	void printIt(QPrinter*);

	void rotate(int, int);
	void changeSpeed(int, int);
	void doRotate();

	void inertia();

protected:
	void timerEvent(QTimerEvent*);
	void repaint3d();
	virtual void rotate(int direction);

public:
	GLWindow();
	GLWidget *glWidget() { return (GLWidget*)this->centralWidget(); }
};

#endif /* __GLWINDOW_H__ */

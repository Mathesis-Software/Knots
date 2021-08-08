#ifndef __GLWINDOW_H__
#define __GLWINDOW_H__

#include <list>
#include <memory>

#include <QtWidgets/QOpenGLWidget>

#include "../abstractWindow/abstractWindow.h"
#include "../../gl/surface/surface.h"

class GLWindow : public abstractWindow {
	Q_OBJECT

private:
	std::list<std::shared_ptr<KE::GL::Surface>> surfaces;
	float backgroundRGB[3];

	double *currentSpeedMatrix;
	double *currentMatrix;

	bool isInertia;
	int timerId_rotate;

	void printIt(QPrinter*);

	void rotate(int, int);
	void changeSpeed(int, int);
	void doRotate();

private slots:
	void inertia();
	void rotate(int);

protected:
	void timerEvent(QTimerEvent*);
	void addSurface(std::shared_ptr<KE::GL::Surface> s) {this->surfaces.push_back(s);};
	double currMatr(int i, int j) {return currentMatrix[4 * i + j];}
	void repaint3d();

	const float *getBackgroundRGB() { return this->backgroundRGB; }
	void setBackgroundRGB(const float rgb[3]) {
		this->backgroundRGB[0] = rgb[0];
		this->backgroundRGB[1] = rgb[1];
		this->backgroundRGB[2] = rgb[2];
	}

public:
	GLWindow();
	~GLWindow();

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
	~GLWidget();

	void multMatrix() {
		makeCurrent();
		glMultMatrixd(Parent->currentMatrix);
	}
};

#endif /* __GLWINDOW_H__ */

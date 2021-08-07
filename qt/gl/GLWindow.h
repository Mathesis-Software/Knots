#ifndef __GLWINDOW_H__
#define __GLWINDOW_H__

#include <list>
#include <memory>

#include <QtWidgets/QOpenGLWidget>

#include "../setupColor/setupColor.h"
#include "../abstractWindow/abstractWindow.h"
#include "../../gl/surface/surface.h"

class GLWindow : public abstractWindow {
	Q_OBJECT

private:
	std::list<std::shared_ptr<KE::GL::Surface>> surfaces;

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
	float backgroundRGB[3];

	void timerEvent(QTimerEvent*);
	void addSurface(std::shared_ptr<KE::GL::Surface> s) {this->surfaces.push_back(s);};
	double currMatr(int i, int j) {return currentMatrix[4 * i + j];}
	void repaint3d();

public:
	GLWindow();
	~GLWindow();

	friend class GLWidget;
	friend class setupGLWidgetColor;
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

class setupGLWidgetColor : public setupColor {

Q_OBJECT

private:
	GLWindow *Parent;

private slots:
	void apply() { Parent->repaint3d(); }
		
public:
	setupGLWidgetColor(GLWindow *p, float *rgb, bool just = true)
		: setupColor(rgb, just) {Parent = p;};
};

#endif /* __GLWINDOW_H__ */

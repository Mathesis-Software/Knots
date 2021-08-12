#ifndef __ABOUT_H__
#define __ABOUT_H__

#include <memory>

#include <QtWidgets/QLabel>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QWidget>

#include "../../gl/surface/surface.h"

class aboutWindow : public QWidget {

public:
	aboutWindow(QWidget*);

private:
	class TrefoilWidget *trefoil;
	int timerId;

	void timerEvent(QTimerEvent*) override;
	void mousePressEvent(QMouseEvent*) override { this->close(); }
	void closeEvent(QCloseEvent*) override { this->killTimer(this->timerId); }
};

class TrefoilSurface : public KE::GL::Surface {

public:
	TrefoilSurface();
	void calculate() {}
};

class TrefoilWidget : public QOpenGLWidget {

public:
	TrefoilWidget(QWidget*);

private:
	std::unique_ptr<TrefoilSurface> surface;
	float x, y, z;

	void resizeGL(int, int);
	void paintGL();
};

#endif /* __ABOUT_H__ */

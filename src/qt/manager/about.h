#ifndef __ABOUT_H__
#define __ABOUT_H__

#include <QtWidgets/QLabel>
#include <QtWidgets/QOpenGLWidget>
#include <QtWidgets/QWidget>

#include "../../gl/surface/surface.h"

class aboutWindow : public QWidget {

public:
	aboutWindow(QWidget*);
	~aboutWindow();

private:
	class pictureWidget *picture;
	QLabel *text;

	void timerEvent(QTimerEvent*);
	void mousePressEvent(QMouseEvent*) { delete this; }

	int timer_id;
};

class aboutSurface : public KE::GL::Surface {

public:
	aboutSurface(const char*);
	void calculate() {}
};

class pictureWidget : public QOpenGLWidget {

public:
	pictureWidget(QWidget*);
	~pictureWidget();

private:
	aboutSurface *aboutSurf;
	float x, y, z;

	void resizeGL(int, int);
	void paintGL();
	void mousePressEvent(QMouseEvent*) { delete parent(); }
};

#endif /* __ABOUT_H__ */

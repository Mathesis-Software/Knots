#include <QtGui/QPainter>

#include "GLWidget.h"
#include "GLWindow.h"

GLWindow::GLWindow() {
	setCentralWidget(new GLWidget(this));
}

GLWidget *GLWindow::glWidget() const {
	return (GLWidget*)this->centralWidget();
}

void GLWindow::printIt(QPrinter *prn) {
	QPainter pnt;
	pnt.begin(prn);
	raise();
	pnt.drawPixmap(0, 0, QPixmap::grabWindow(centralWidget()->winId()));
	pnt.end();
}

#include <QtGui/QPixmap>
#include <QtGui/QPainter>
#include <QtWidgets/QToolBar>

#include "GLWindow.h"

GLWindow::GLWindow() {
	setCentralWidget(new GLWidget(this));

	for (int i = 0; i < 6; i++) {
		this->addToolbarAction(QString("rotate%1.xpm").arg(i), "Rotate", [this, i] { this->rotate(i); });
	}
}

void GLWindow::printIt(QPrinter *prn) {
	QPainter pnt;
	pnt.begin(prn);
	raise();
	pnt.drawPixmap(0, 0, QPixmap::grabWindow(centralWidget()->winId()));
	pnt.end();
}

void GLWindow::repaint3d() {
	if (isVisible()) {
		((GLWidget*)centralWidget())->update();
	}
}

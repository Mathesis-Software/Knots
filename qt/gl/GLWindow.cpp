#include <QtGui/QPixmap>
#include <QtGui/QPainter>
#include <QtWidgets/QToolBar>

#include "GLWindow.h"

GLWindow::GLWindow() : currentMatrix(new double[16]), currentSpeedMatrix(new double[9]) {
	setCentralWidget(new GLWidget(this));

	backgroundRGB[0] = 1.0;
	backgroundRGB[1] = 1.0;
	backgroundRGB[2] = 1.0;

	{
		int i;
		for (i = 0; i < 16; i++)
			currentMatrix[i] = (i % 5) ? 0.0 : 1.0;
		for (i = 0; i < 9; i++)
			currentSpeedMatrix[i] = (i % 4) ? 0.0 : 1.0;
	}
	isInertia = false;

	for (int i = 0; i < 6; i++) {
		this->addToolbarAction(QString("rotate%1.xpm").arg(i), "Rotate", [this, i] { this->rotate(i); });
	}

	addToolBarSeparator();
	addToolbarAction("inertia.xpm", "Inertia", [this] { this->inertia(); })->setCheckable(true);
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

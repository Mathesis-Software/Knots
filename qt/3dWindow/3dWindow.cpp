#include <QtGui/QPixmap>
#include <QtGui/QPainter>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QButtonGroup>

#include "3dWindow.h"

dddWindow::dddWindow() {
	setCentralWidget(new dddMainWidget(this));

	backgroundRGB [0] = 1.0;
	backgroundRGB [1] = 1.0;
	backgroundRGB [2] = 1.0;

	currentMatrix = new double [16];
	currentSpeedMatrix = new double [9];
	{
		int i;
		for (i = 0; i < 16; i++)
			currentMatrix [i] = (i % 5) ? 0.0 : 1.0;
		for (i = 0; i < 9; i++)
			currentSpeedMatrix [i] = (i % 4) ? 0.0 : 1.0;
	}
	isInertia = false;

	QButtonGroup *grp = new QButtonGroup;
	for (int i = 0; i < 6; i++) {
		QToolButton *button = addToolBarButton(QString("rotate%1.xpm").arg(i), "Rotate", 0);
		grp->addButton(button);
		this->connect(button, &QToolButton::pressed, [=](){ this->rotate(i); });
	}

	addToolBarSeparator();
	addToolBarButton("inertia.xpm", "Inertia", SLOT(inertia()))->setCheckable(true);
}

dddWindow::~dddWindow() {
	delete[] currentMatrix;
	delete[] currentSpeedMatrix;
}

void dddWindow::printIt(QPrinter *prn) {
	QPainter pnt;
	pnt.begin(prn);
	raise();
	pnt.drawPixmap(0, 0, QPixmap::grabWindow(centralWidget()->winId()));
	pnt.end();
}

void dddWindow::repaint3d() {
	if (isVisible()) {
		((dddMainWidget*)centralWidget())->update();
	}
}

#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>

#include "about.h"

AboutWindow::AboutWindow(QWidget *parent) : QWidget(parent) {
	this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
	this->setAttribute(Qt::WA_DeleteOnClose);
	this->setWindowModality(Qt::ApplicationModal);
	setFixedSize(380, 180);
	QPalette pal = this->palette();
	pal.setColor(QPalette::Background, Qt::white);
	this->setAutoFillBackground(true);
	this->setPalette(pal);
	this->timerId = this->startTimer(100);

	this->trefoil = new TrefoilWidget(this);
	this->trefoil->setGeometry(40, 30, 120, 120);
	this->trefoil->update();

	auto text = new QLabel(this);
	text->setGeometry(180, 30, 150, 120);
	QFont fnt("Helvetica", 14);
	text->setFont(fnt);
	text->setAlignment(Qt::AlignCenter);
	text->setText(QString("Knot Editor\nversion ") + VERSION);
}

void AboutWindow::showMe() {
	const QRect screenGeometry = QApplication::desktop()->screenGeometry();
	const int x = (screenGeometry.width() - 380) / 2;
	const int y = (screenGeometry.height() - 180) / 2;
	this->move(x, y);
	this->show();
}

void AboutWindow::timerEvent(QTimerEvent*) {
	this->trefoil->update();
}

TrefoilSurface::TrefoilSurface() : Surface(true, false) {
	QFile file(":images/about.srf");
	file.open(QIODevice::ReadOnly);
	QByteArray data = file.readAll();
	file.close();
	QTextStream is(data);

	float p[6];

	while (!is.atEnd()) {
		is >> p[0] >> p[1] >> p[2] >> p[3] >> p[4] >> p[5];
		if (is.status() != QTextStream::Ok) {
			break;
		}
		addpoint(KE::ThreeD::Point(p[0], p[1], p[2]), KE::ThreeD::Vector(p[3], p[4], p[5]));
	}

	this->show();
}

TrefoilWidget::TrefoilWidget(QWidget *parent) : QOpenGLWidget(parent), surface(new TrefoilSurface()) {
	srandom(time(NULL));
	x = 1.0;
	y = 0.0;
	z = 0.0;
}

void TrefoilWidget::initializeGL() {
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}

void TrefoilWidget::resizeGL(int w, int h) {
	glViewport(0, 0, w, h);

	float pos[] = {5.0, 5.0, 10.0, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	glEnable(GL_LIGHT0);

	glEnable(GL_LIGHTING);

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-0.7, 0.7, -0.7, 0.7, 5, 25);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -10.0);

	glClearColor(1.0, 1.0, 1.0, 1.0);
}

void TrefoilWidget::paintGL() {
	x += 0.4 * random() / RAND_MAX - 0.2;
	y += 0.4 * random() / RAND_MAX - 0.2;
	z += 0.4 * random() / RAND_MAX - 0.2;
	float len = sqrt(x * x + y * y + z * z);
	x /= len;
	y /= len;
	z /= len;

	makeCurrent();
	glRotatef(5.0, x, y, z);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	this->surface->paint();
	glFlush();
}

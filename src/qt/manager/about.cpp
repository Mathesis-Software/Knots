#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QLabel>

#include "about.h"

namespace KE { namespace Qt {

void AboutWindow::showAboutDialog() {
	(new AboutWindow(nullptr))->showMe();
}

AboutWindow::AboutWindow(QWidget *parent) : QWidget(parent) {
	this->setWindowFlags(::Qt::Window | ::Qt::FramelessWindowHint | ::Qt::WindowStaysOnTopHint);
	this->setAttribute(::Qt::WA_DeleteOnClose);
	this->setWindowModality(::Qt::ApplicationModal);
	setFixedSize(380, 180);
	QPalette pal = this->palette();
	pal.setColor(QPalette::Background, ::Qt::white);
	this->setAutoFillBackground(true);
	this->setPalette(pal);

	auto icon = new QLabel(this);
	icon->setGeometry(36, 26, 128, 128);
	QPixmap pixmap(":images/trefoil.png");
	const auto dpr = this->devicePixelRatio();
	pixmap.setDevicePixelRatio(dpr);
	icon->setPixmap(pixmap.scaled(128 * dpr, 128 * dpr, ::Qt::IgnoreAspectRatio, ::Qt::SmoothTransformation));

	auto text = new QLabel(this);
	text->setGeometry(180, 30, 150, 120);
	QFont fnt("Helvetica", 14);
	text->setFont(fnt);
	text->setAlignment(::Qt::AlignCenter);
	text->setText(QString("Knot Editor\nversion ") + VERSION);
}

void AboutWindow::showMe() {
	const QRect screenGeometry = QApplication::desktop()->screenGeometry();
	const int x = (screenGeometry.width() - 380) / 2;
	const int y = (screenGeometry.height() - 180) / 2;
	this->move(x, y);
	this->show();
}

void AboutWindow::mousePressEvent(QMouseEvent*) {
	this->close();
}

}}

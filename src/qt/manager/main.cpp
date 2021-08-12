#include <QtGui/QPainter>
#include <QtWidgets/QApplication>
#include <QtWidgets/QProxyStyle>

#include "manager.h"

class ProxyStyle : public QProxyStyle {

QPixmap generatedIconPixmap(QIcon::Mode iconMode, const QPixmap &pixmap, const QStyleOption *option) const override {
	if (iconMode == QIcon::Disabled) {
		QPixmap copy(pixmap);
		QPainter painter(&copy);
		painter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
		painter.fillRect(copy.rect(), 0xc0c0c0);
		painter.end();
		return copy;
	}
	return QProxyStyle::generatedIconPixmap(iconMode, pixmap, option);
}

};

int main(int argc, char **argv) {
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
	QApplication qa(argc, argv);
	qa.setFont(QFont("Helvetica", 10));
	qa.setStyle(new ProxyStyle);

	QPixmap pixmap(":images/trefoil.png");
	pixmap.setDevicePixelRatio(qa.devicePixelRatio());
	qa.setWindowIcon(pixmap);

	keManager kem;
	kem.show();
	return qa.exec();
}

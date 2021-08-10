#include <QtWidgets/QApplication>

#include "manager.h"

int main(int argc, char **argv) {
	QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
	QApplication qa(argc, argv);
	qa.setFont(QFont("Helvetica", 11));

	keManager kem;
	kem.show();
	return qa.exec();
}

#include <cmath>

#include <QtWidgets/QColorDialog>

#include "../gl/GLWidget.h"
#include "knotWindow.h"

#include "../../math/knotSurface/KnotSurface.h"
#include "../../math/seifert/SeifertSurface.h"

namespace {

QColor fromRGB(const float *rgb) {
	return QColor((int)round(rgb[0] * 255), (int)round(rgb[1] * 255), (int)round(rgb[2] * 255));
}
QColor fromRGB(const KE::GL::Color &color) {
	return QColor((int)round(color.rgb[0] * 255), (int)round(color.rgb[1] * 255), (int)round(color.rgb[2] * 255));
}

}

void knotWindow::runColorDialog(const QString &title, std::function<QColor()> getter, std::function<void(const QColor&)> setter) {
	const QColor initial = getter();
	QColorDialog dialog(initial, nullptr);
	dialog.setWindowTitle(title);
	connect(&dialog, &QColorDialog::currentColorChanged, setter);
	if (dialog.exec() == QColorDialog::Rejected) {
		setter(initial);
	}
}

void knotWindow::setBgColor() {
	this->runColorDialog(
		"Background Color",
		[this] { return fromRGB(this->glWidget()->getBackgroundRGB()); },
		[this] (const QColor &color) {
			const float rgb[3] = { color.red() / 255.0f, color.green() / 255.0f, color.blue() / 255.0f };
			this->glWidget()->setBackgroundRGB(rgb);
			this->centralWidget()->update();
		}
	);
}

void knotWindow::setKnotColor() {
	this->runColorDialog(
		"Knot Color",
		[this] { return fromRGB(this->knotSurface->frontColor()); },
		[this] (const QColor &color) {
			this->knot.knotColor = std::make_shared<KE::GL::Color>(color.red(), color.green(), color.blue());
			this->centralWidget()->update();
		}
	);
}

void knotWindow::setSeifertFrontColor() {
	this->runColorDialog(
		"Seifert Surface Color",
		[this] { return fromRGB(this->seifertSurface->frontColor()); },
		[this] (const QColor &color) {
			this->knot.seifertFrontColor = std::make_shared<KE::GL::Color>(color.red(), color.green(), color.blue());
			this->centralWidget()->update();
		}
	);
}

void knotWindow::setSeifertBackColor() {
	this->runColorDialog(
		"Seifert Surface Back Side Color",
		[this] { return fromRGB(this->seifertSurface->backColor()); },
		[this] (const QColor &color) {
			this->knot.seifertBackColor = std::make_shared<KE::GL::Color>(color.red(), color.green(), color.blue());
			this->centralWidget()->update();
		}
	);
}

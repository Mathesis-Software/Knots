#include <cmath>

#include <QtWidgets/QColorDialog>

#include "knotWindow.h"

#include "../../math/knot/KnotSurface.h"
#include "../../math/seifert/seifert_surface.h"

namespace {

QColor fromRGB(const float *rgb) {
	return QColor((int)round(rgb[0] * 255), (int)round(rgb[1] * 255), (int)round(rgb[2] * 255));
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
			this->repaint3d();
		}
	);
}

void knotWindow::setKnotColor() {
	this->runColorDialog(
		"Knot Color",
		[this] { return fromRGB(this->knotSurface->getFrontRGB()); },
		[this] (const QColor &color) {
			const float rgb[3] = { color.red() / 255.0f, color.green() / 255.0f, color.blue() / 255.0f };
			this->knotSurface->setFrontRGB(rgb);
			this->repaint3d();
		}
	);
}

void knotWindow::setSeifertFrontColor() {
	this->runColorDialog(
		"Seifert Surface Color",
		[this] { return fromRGB(this->seifertSurface->getFrontRGB()); },
		[this] (const QColor &color) {
			const float rgb[3] = { color.red() / 255.0f, color.green() / 255.0f, color.blue() / 255.0f };
			this->seifertSurface->setFrontRGB(rgb);
			this->repaint3d();
		}
	);
}

void knotWindow::setSeifertBackColor() {
	this->runColorDialog(
		"Seifert Surface Back Side Color",
		[this] { return fromRGB(this->seifertSurface->getBackRGB()); },
		[this] (const QColor &color) {
			const float rgb[3] = { color.red() / 255.0f, color.green() / 255.0f, color.blue() / 255.0f };
			this->seifertSurface->setBackRGB(rgb);
			this->repaint3d();
		}
	);
}

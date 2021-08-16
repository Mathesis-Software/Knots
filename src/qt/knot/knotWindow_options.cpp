#include <QtWidgets/QColorDialog>

#include "knotWindow.h"
#include "KnotWidget.h"
#include "../setValue/setValue.h"
#include "../../math/knotSurface/KnotSurface.h"
#include "../../math/seifert/SeifertSurface.h"

void knotWindow::setThickness() {
  const double thickness = setDouble("Thickness", this->knotSurface->thickness(), 0.1, 10.0);
  if (thickness != this->knotSurface->thickness()) {
		this->knot.knotThickness = std::make_shared<double>(thickness);
    this->knotSurface->destroy(true);
		this->centralWidget()->update();
  }
}

namespace {

QColor from(const KE::GL::Color &color) {
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
		[this] { return from(this->knotWidget()->backgroundColor()); },
		[this] (const QColor &color) {
			this->knot.backgroundColor = std::make_shared<KE::GL::Color>(color.red(), color.green(), color.blue());
			this->centralWidget()->update();
		}
	);
}

void knotWindow::setKnotColor() {
	this->runColorDialog(
		"Knot Color",
		[this] { return from(this->knotSurface->frontColor()); },
		[this] (const QColor &color) {
			this->knot.knotColor = std::make_shared<KE::GL::Color>(color.red(), color.green(), color.blue());
			this->centralWidget()->update();
		}
	);
}

void knotWindow::setSeifertFrontColor() {
	this->runColorDialog(
		"Seifert Surface Color",
		[this] { return from(this->seifertSurface->frontColor()); },
		[this] (const QColor &color) {
			this->knot.seifertFrontColor = std::make_shared<KE::GL::Color>(color.red(), color.green(), color.blue());
			this->centralWidget()->update();
		}
	);
}

void knotWindow::setSeifertBackColor() {
	this->runColorDialog(
		"Seifert Surface Back Side Color",
		[this] { return from(this->seifertSurface->backColor()); },
		[this] (const QColor &color) {
			this->knot.seifertBackColor = std::make_shared<KE::GL::Color>(color.red(), color.green(), color.blue());
			this->centralWidget()->update();
		}
	);
}

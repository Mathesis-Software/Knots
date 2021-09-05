/*
 * Copyright (c) 1995-2021, Nikolay Pultsin <geometer@geometer.name>
 *
 * Licensed under the Apache License, Version 2.0 the "License";
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <QtCore/QMetaMethod>
#include <QtGui/QDoubleValidator>
#include <QtGui/QPainter>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QColorDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

#include "KnotWidget.h"
#include "KnotWindow.h"

namespace KE::Qt {

namespace {

class ColorButton : public QPushButton {

private:
	const int _width = 30;
	const int _height = 20;
	const int _delta = 10;

	const std::function<void(const Color&)> setter;

public:
	QColor currentColor;

public:
	ColorButton(const QString &title, const Color &initialColor, std::function<void(const Color&)> setter) : setter(setter) {
		this->setColor(QColor(initialColor.red(), initialColor.green(), initialColor.blue()));
		this->setSizePolicy(QSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum));
		this->setMinimumSize(_width + _delta, _height + _delta);
		this->setIconSize(QSize(_width, _height));
		this->setAutoDefault(false);
		QObject::connect(this, &QPushButton::clicked, [=] {
			const QColor initial = this->currentColor;
			QColorDialog dialog(initial, nullptr);
			dialog.setWindowFlags(::Qt::CustomizeWindowHint | ::Qt::WindowTitleHint);
			dialog.setWindowTitle(title);
			QObject::connect(&dialog, &QColorDialog::currentColorChanged, [=](const QColor &color) {
				this->setColor(color, true);
			});
			if (dialog.exec() == QColorDialog::Rejected) {
				this->setColor(initial, true);
			}
		});
	}

	void setColor(const QColor &color, bool runSetter = false) {
		if (color == this->currentColor) {
			return;
		}

		this->currentColor = color;
		if (runSetter) {
			this->setter(Color(color.red(), color.green(), color.blue()));
		}
		const auto dpr = this->devicePixelRatio();
		QPixmap image(_width * dpr, _height * dpr);
		image.setDevicePixelRatio(dpr);
		image.fill(color);
		QPainter pnt;
		pnt.begin(&image);
		pnt.setPen(::Qt::black);
		pnt.drawRect(0, 0, _width, _height);
		pnt.end();
		this->setIcon(image);
	}
};

template<class W> struct WidgetWithLabel {
	W *widget;
	QLabel *label;

	WidgetWithLabel(W *widget, const QString &title) : widget(widget), label(new QLabel(title)) {}

	void setEnabled(bool enabled) const {
		this->widget->setEnabled(enabled);
		this->label->setEnabled(enabled);
	}

	void addToLayout(QGridLayout *layout) const {
		const int row = layout->rowCount();
		layout->addWidget(this->label, row, 0);
		layout->addWidget(this->widget, row, 1);
		layout->setRowMinimumHeight(row, 30);
	}
};

WidgetWithLabel<ColorButton> addColorButton(const QString &title, const Color &initialColor, std::function<void(const Color&)> setter) {
	return WidgetWithLabel(new ColorButton(title, initialColor, setter), title);
}

}

void KnotWindow::showOptionsDialog() {
	if (this->isSignalConnected(QMetaMethod::fromSignal(&KnotWindow::raiseOptionsDialog))) {
		emit raiseOptionsDialog();
		return;
	}

	auto options = new KnotOptionsDialog(*this);
	options->setWindowTitle("Visual options for " + this->windowTitle());
	options->show();
	const QRect geometry = this->geometry();
	const QRect optionsGeometry = options->geometry();
	const int x = geometry.right() + 10;
	const int y = geometry.y() + (geometry.height() - optionsGeometry.height()) / 2;
	options->move(x, y);
}

KnotOptionsDialog::KnotOptionsDialog(KnotWindow &window) {
	this->setAttribute(::Qt::WA_DeleteOnClose);
	this->setWindowFlags(::Qt::CustomizeWindowHint | ::Qt::WindowTitleHint);

	auto widget = window.knotWidget();

	const auto initialThickness = widget->knot.knotThickness();
	const auto initialBackgroundColor = widget->knot.backgroundColor();
	const auto initialKnotColor = widget->knot.knotColor();
	const auto initialSeifertVisibility = widget->knot.isSeifertSurfaceVisible();
	const auto initialSeifertFrontColor = widget->knot.seifertFrontColor();
	const auto initialSeifertBackColor = widget->knot.seifertBackColor();

	auto vbox = new QVBoxLayout(this);
	auto layout = new QGridLayout();
	vbox->addLayout(layout);
	vbox->addSpacing(30);
	auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	QObject::connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	QObject::connect(buttonBox, &QDialogButtonBox::rejected, [=] {
		widget->knot.setKnotThickness(initialThickness);
		widget->knot.setBackgroundColor(initialBackgroundColor);
		widget->knot.setKnotColor(initialKnotColor);
		widget->setSeifertSurfaceVisibility(initialSeifertVisibility);
		widget->knot.setSeifertFrontColor(initialSeifertFrontColor);
		widget->knot.setSeifertBackColor(initialSeifertBackColor);
		widget->update();
		this->reject();
	});
	vbox->addWidget(buttonBox);
	vbox->setSizeConstraint(QLayout::SetFixedSize);

	auto thickness = new QLineEdit();
	thickness->setValidator(new QDoubleValidator(0.01, 20, 5));
	thickness->setText(QString::number(initialThickness));
	QObject::connect(thickness, &QLineEdit::textChanged, [widget, thickness, buttonBox](const QString &value) {
		bool ok;
		const double doubleValue = value.toDouble(&ok);
		if (ok && doubleValue >= 0.01 && doubleValue <= 20) {
			thickness->setStyleSheet(QString());
			buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
			widget->knot.setKnotThickness(doubleValue);
			widget->update();
		} else {
			thickness->setStyleSheet("border: 1px solid red;");
			thickness->setToolTip("Thickness must be a number between 0.01 and 20");
			buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
		}
		thickness->update();
	});
	WidgetWithLabel(thickness, "Knot thickness").addToLayout(layout);

	addColorButton("Background color", initialBackgroundColor, [widget](const Color &color) {
		widget->knot.setBackgroundColor(color);
		widget->update();
	}).addToLayout(layout);
	addColorButton("Knot color", initialKnotColor, [widget](const Color &color) {
		widget->knot.setKnotColor(color);
		widget->update();
	}).addToLayout(layout);

	WidgetWithLabel seifertVisibility(new QCheckBox(QString()), "Show Seifert surface");
	WidgetWithLabel sameColor(new QCheckBox(QString()), "Same color for front & back");
	sameColor.widget->setChecked(initialSeifertFrontColor == initialSeifertBackColor);

	auto seifertBack = addColorButton("Seifert surface back color", initialSeifertBackColor, [widget](const Color &color) {
		widget->knot.setSeifertBackColor(color);
		widget->update();
	});

	auto seifertFront = addColorButton("Seifert surface color", initialSeifertFrontColor, [=](const Color &color) {
		widget->knot.setSeifertFrontColor(color);
		if (sameColor.widget->isChecked()) {
			widget->knot.setSeifertBackColor(color);
			seifertBack.widget->setColor(QColor(color.red(), color.green(), color.blue()));
		}
		widget->update();
	});

	seifertVisibility.addToLayout(layout);
	seifertFront.addToLayout(layout);
	sameColor.addToLayout(layout);
	seifertBack.addToLayout(layout);

	QObject::connect(seifertVisibility.widget, &QCheckBox::stateChanged, [=](int state) {
		const bool visible = state == ::Qt::Checked;
		widget->setSeifertSurfaceVisibility(visible);
		seifertFront.setEnabled(visible);
		sameColor.setEnabled(visible);
		seifertBack.setEnabled(visible && !sameColor.widget->isChecked());
	});
	QObject::connect(sameColor.widget, &QCheckBox::stateChanged, [=](int state) {
		const bool same = state == ::Qt::Checked;
		seifertBack.setEnabled(!same && seifertVisibility.widget->isChecked());
		if (same) {
			seifertBack.widget->setColor(seifertFront.widget->currentColor, true);
		}
	});
	seifertVisibility.widget->setChecked(initialSeifertVisibility);
	seifertFront.setEnabled(initialSeifertVisibility);
	sameColor.setEnabled(initialSeifertVisibility);
	seifertBack.setEnabled(initialSeifertVisibility && !sameColor.widget->isChecked());

	layout->setSizeConstraint(QLayout::SetFixedSize);
	QObject::connect(&window, &Window::closing, this, &QDialog::close);
	QObject::connect(&window, &KnotWindow::raiseOptionsDialog, this, &QDialog::raise);
}

}

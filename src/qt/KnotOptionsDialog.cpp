/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at

 *   http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 * Author: Nikolay Pultsin <geometer@geometer.name>
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

#include "KnotOptionsDialog.h"
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
	QColor currentColor;

public:
	ColorButton(const QString &title, const Color &initialColor, std::function<void(const Color&)> setter) : setter(setter) {
		this->setColor(QColor(initialColor.red(), initialColor.green(), initialColor.blue()), false);
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

private:
	void setColor(const QColor &color, bool runSetter) {
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

struct ColorButtonWithLabel {
	ColorButton *button;
	QLabel *label;

	ColorButtonWithLabel(ColorButton *button, QLabel *label) : button(button), label(label) {
	}

	void setEnabled(bool enabled) const {
		button->setEnabled(enabled);
		label->setEnabled(enabled);
	}
};

ColorButtonWithLabel addColorButton(QGridLayout *layout, const QString &title, const Color &initialColor, std::function<void(const Color&)> setter) {
	const int row = layout->rowCount();
	auto label = new QLabel(title);
	layout->addWidget(label, row, 0);
	auto button = new ColorButton(title, initialColor, setter);
	layout->addWidget(button, row, 1);
	return ColorButtonWithLabel(button, label);
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

	const auto initialThickness = widget->knot().knotThickness();
	const auto initialBackgroundColor = widget->knot().backgroundColor();
	const auto initialKnotColor = widget->knot().knotColor();
	const auto initialSeifertVisibility = widget->isSeifertSurfaceVisible();
	const auto initialSeifertFrontColor = widget->knot().seifertFrontColor();
	const auto initialSeifertBackColor = widget->knot().seifertBackColor();

	auto vbox = new QVBoxLayout(this);
	auto layout = new QGridLayout();
	vbox->addLayout(layout);
	vbox->addSpacing(30);
	auto buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	QObject::connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
	QObject::connect(buttonBox, &QDialogButtonBox::rejected, [=] {
		widget->_knot.setKnotThickness(initialThickness);
		widget->_knot.setBackgroundColor(initialBackgroundColor);
		widget->_knot.setKnotColor(initialKnotColor);
		widget->setSeifertSurfaceVisibility(initialSeifertVisibility);
		widget->_knot.setSeifertFrontColor(initialSeifertFrontColor);
		widget->_knot.setSeifertBackColor(initialSeifertBackColor);
		widget->update();
		this->reject();
	});
	vbox->addWidget(buttonBox);
	vbox->setSizeConstraint(QLayout::SetFixedSize);

	layout->addWidget(new QLabel("Knot thickness"), 0, 0);
	auto thickness = new QLineEdit();
	thickness->setValidator(new QDoubleValidator(0.01, 20, 5));
	thickness->setText(QString::number(initialThickness));
	QObject::connect(thickness, &QLineEdit::textChanged, [widget, thickness, buttonBox](const QString &value) {
		bool ok;
		const double doubleValue = value.toDouble(&ok);
		if (ok && doubleValue >= 0.01 && doubleValue <= 20) {
			thickness->setStyleSheet(QString());
			buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
			widget->_knot.setKnotThickness(doubleValue);
			widget->update();
		} else {
			thickness->setStyleSheet("border: 1px solid red;");
			thickness->setToolTip("Thickness must be a number between 0.01 and 20");
			buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
		}
		thickness->update();
	});
	layout->addWidget(thickness, 0, 1);

	addColorButton(layout, "Background color", initialBackgroundColor, [widget](const Color &color) {
		widget->_knot.setBackgroundColor(color);
		widget->update();
	});
	addColorButton(layout, "Knot color", initialKnotColor, [widget](const Color &color) {
		widget->_knot.setKnotColor(color);
		widget->update();
	});

	const int row = layout->rowCount();
	layout->addWidget(new QLabel("Show Seifert surface"), row, 0);
	auto seifertVisibility = new QCheckBox(QString());
	layout->addWidget(seifertVisibility, row, 1);
	auto seifertFront = addColorButton(layout, "Seifert surface color", initialSeifertFrontColor, [widget](const Color &color) {
		widget->_knot.setSeifertFrontColor(color);
		widget->update();
	});
	auto seifertBack = addColorButton(layout, "Seifert surface back color", initialSeifertBackColor, [widget](const Color &color) {
		widget->_knot.setSeifertBackColor(color);
		widget->update();
	});
	QObject::connect(seifertVisibility, &QCheckBox::stateChanged, [widget, seifertFront, seifertBack](int state) {
		const bool visible = state == ::Qt::Checked;
		widget->setSeifertSurfaceVisibility(visible);
		seifertFront.setEnabled(visible);
		seifertBack.setEnabled(visible);
	});
	seifertVisibility->setChecked(initialSeifertVisibility);
	seifertFront.setEnabled(initialSeifertVisibility);
	seifertBack.setEnabled(initialSeifertVisibility);

	for (int row = 0; row < layout->rowCount(); row += 1) {
		layout->setRowMinimumHeight(row, 30);
	}

	layout->setSizeConstraint(QLayout::SetFixedSize);
	QObject::connect(&window, &Window::closing, this, &QDialog::close);
	QObject::connect(&window, &KnotWindow::raiseOptionsDialog, this, &QDialog::raise);
}

}
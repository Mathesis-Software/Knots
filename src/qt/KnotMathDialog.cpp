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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>

#include "KnotWidget.h"
#include "KnotWindow.h"
#include "../math/computables.h"
#include "../math/experimental.h"

namespace KE::Qt {

void KnotWindow::showMathDialog() {
	if (this->isSignalConnected(QMetaMethod::fromSignal(&KnotWindow::raiseMathDialog))) {
		emit raiseMathDialog();
		return;
	}

	auto mth = new KnotMathDialog(*this);
	mth->setWindowTitle("Computables for " + this->windowTitle());
	mth->show();
	const QRect geometry = this->geometry();
	const QRect mthGeometry = mth->geometry();
	const int x = geometry.right();
	const int y = geometry.y() + (geometry.height() - mthGeometry.height()) / 2;
	mth->move(x, y);
}

KnotMathDialog::KnotMathDialog(KnotWindow &window) {
	this->setAttribute(::Qt::WA_DeleteOnClose);

	auto layout = new QGridLayout(this);

	const auto &knot = window.knotWidget()->knot;
	std::vector<std::shared_ptr<ThreeD::Math::Computable>> computables = {
		std::make_shared<ThreeD::Math::MoebiusEnergy>(knot),
		std::make_shared<ThreeD::Math::AverageCrossingNumber>(knot, false),
		std::make_shared<ThreeD::Math::AverageCrossingNumber>(knot, true),
		std::make_shared<ThreeD::Math::AverageExtremumNumber>(knot),
		std::make_shared<ThreeD::Math::VassilievInvariant>(knot, 2),
		std::make_shared<ThreeD::Math::VassilievInvariant>(knot, 3),
		std::make_shared<ThreeD::Math::VassilievInvariant>(knot, 4),
		std::make_shared<ThreeD::Math::VassilievInvariant>(knot, 5),
		std::make_shared<ThreeD::Math::Experimental>(knot)
//		std::make_shared<ThreeD::Math::Singular>(knot),
//		std::make_shared<ThreeD::Math::Experimental2>(knot, 2, "Experimental 2"),
//		std::make_shared<ThreeD::Math::Experimental2>(knot, 3, "Experimental 3"),
//		std::make_shared<ThreeD::Math::Experimental2>(knot, 4, "Experimental 4"),
	};
	for (const auto &computable : computables) {
		const int index = layout->rowCount();
		auto checkbox = new QCheckBox(computable->name.c_str());
		layout->addWidget(checkbox, index, 0);

		auto value = new QLabel();
		value->setTextInteractionFlags(::Qt::TextSelectableByMouse);
		value->setFrameStyle(QFrame::Panel | QFrame::Sunken);
		value->setMinimumWidth(100);
		layout->addWidget(value, index, 1);
		layout->setRowMinimumHeight(index, 30);

		const auto callback = [checkbox, value, computable] {
			if (checkbox->isChecked()) {
				value->setText(QString::number(computable->value()));
			} else {
				value->setText(QString());
			}
		};
		QObject::connect(&window, &Window::contentChanged, this, callback);
		QObject::connect(checkbox, &QCheckBox::clicked, this, callback);
	}

	layout->setSizeConstraint(QLayout::SetFixedSize);

	QObject::connect(&window, &Window::closing, this, &QDialog::close);
	QObject::connect(&window, &KnotWindow::raiseMathDialog, this, &QDialog::raise);
}

}

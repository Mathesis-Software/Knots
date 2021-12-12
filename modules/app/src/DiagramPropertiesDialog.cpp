/*
 * Copyright (c) 1995-2021, Mathesis Software <mad@mathesis.fun>
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

#include <sstream>

#include <QtCore/QMetaMethod>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>

#include <DiagramProperty.h>
#include <Polynomial.h>

#include "DiagramWindow.h"

namespace KE::Qt {

void DiagramWindow::showPropertiesDialog() {
	if (auto dialog = this->findChild<DiagramPropertiesDialog*>()) {
		dialog->show();
		dialog->raise();
		dialog->activateWindow();
		return;
	}

	auto mth = new DiagramPropertiesDialog(this);
	mth->setWindowTitle("Properties for " + this->windowTitle());
	mth->show();
	const QRect geometry = this->geometry();
	const QRect mthGeometry = mth->geometry();
	const int x = geometry.right();
	const int y = geometry.y() + (geometry.height() - mthGeometry.height()) / 2;
	mth->move(x, y);
}

DiagramPropertiesDialog::DiagramPropertiesDialog(DiagramWindow *parent) : QDialog(parent) {
	this->setAttribute(::Qt::WA_DeleteOnClose);

	auto layout = new QGridLayout(this);

	layout->addWidget(new QLabel("Dowker-Thistlethwaite code"), 0, 0);
	auto dtCode = new QLabel();
	dtCode->setTextInteractionFlags(::Qt::TextSelectableByMouse);
	dtCode->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	dtCode->setMinimumWidth(200);
	layout->addWidget(dtCode, 0, 1);
	layout->setRowMinimumHeight(0, 30);

	layout->addWidget(new QLabel("Alexander polynomial"), 1, 0);
	auto ap = new QLabel();
	ap->setTextInteractionFlags(::Qt::TextSelectableByMouse);
	ap->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	ap->setMinimumWidth(200);
	layout->addWidget(ap, 1, 1);
	layout->setRowMinimumHeight(1, 30);

	auto callback = [dtCode, ap, parent] {
		const auto &diagram = parent->diagramWidget()->diagram.diagram();
		TwoD::Math::DTCode code;
		if (code.isApplicable(diagram)) {
			std::stringstream stream;
			bool first = true;
			for (const int num : code.value(diagram)) {
				if (first) {
					first = false;
				} else {
					stream << " ";
				}
				stream << num;
			}
			dtCode->setText(stream.str().c_str());
		} else {
			dtCode->setText(QString());
		}
		TwoD::Math::AlexanderPolynomial poly;
		if (poly.isApplicable(diagram)) {
			std::stringstream stream;
			stream << poly.value(diagram);
			ap->setText(stream.str().c_str());
		} else {
			ap->setText(QString());
		}
	};
	QObject::connect(parent->diagramWidget(), &DiagramWidget::diagramChanged, this, callback);
	callback();

	layout->setSizeConstraint(QLayout::SetFixedSize);
}

}

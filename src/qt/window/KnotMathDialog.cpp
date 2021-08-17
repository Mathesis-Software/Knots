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

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>

#include "KnotMathDialog.h"
#include "KnotWindow.h"
#include "../../math/computables/computables.h"
#include "../../math/computables/experimental.h"
#include "../widget/KnotWidget.h"

namespace KE { namespace Qt {

void KnotWindow::math() {
	if (mth) {
		mth->show();
		mth->raise();
		return;
	}

	mth = new KnotMathDialog (this);
	mth->setWindowTitle ((QString) "Parameters for " + windowTitle ());
	mth->show ();
}

KnotMathDialog::KnotMathDialog (KnotWindow *p) {
	Parent = p;
	this->setAttribute(::Qt::WA_DeleteOnClose);

	auto layout = new QGridLayout(this);

	const auto &knot = p->knotWidget()->knot();
	std::vector<std::shared_ptr<ThreeD::Computables::Computable>> computables = {
		std::make_shared<ThreeD::Computables::MoebiusEnergy>(knot),
		std::make_shared<ThreeD::Computables::AverageCrossingNumber>(knot, false),
		std::make_shared<ThreeD::Computables::AverageCrossingNumber>(knot, true),
		std::make_shared<ThreeD::Computables::AverageExtremumNumber>(knot),
		std::make_shared<ThreeD::Computables::VassilievInvariant>(knot, 2),
		std::make_shared<ThreeD::Computables::VassilievInvariant>(knot, 3),
		std::make_shared<ThreeD::Computables::VassilievInvariant>(knot, 4),
		std::make_shared<ThreeD::Computables::VassilievInvariant>(knot, 5),
		std::make_shared<ThreeD::Computables::Experimental>(knot)
//		std::make_shared<ThreeD::Computables::Singular>(knot),
//		std::make_shared<ThreeD::Computables::Experimental2>(knot, 2, "Experimental 2"),
//		std::make_shared<ThreeD::Computables::Experimental2>(knot, 3, "Experimental 3"),
//		std::make_shared<ThreeD::Computables::Experimental2>(knot, 4, "Experimental 4"),
	};
	for (std::size_t index = 0; index < computables.size(); index += 1) {
		const auto computable = computables[index];

		auto checkbox = new QCheckBox(computable->name.c_str());
		layout->addWidget(checkbox, index, 0);

		auto value = new QLabel();
		value->setFrameStyle(QFrame::Panel | QFrame::Sunken);
		value->setMinimumWidth(100);
		layout->addWidget(value, index, 1);

		const auto callback = [checkbox, value, computable] {
			if (checkbox->isChecked()) {
				value->setText(QString::number(computable->value()));
			} else {
				value->setText(QString());
			}
		};
		this->callbacks.push_back(callback);
		QObject::connect(checkbox, &QCheckBox::clicked, callback);
	}

	layout->setSizeConstraint(QLayout::SetFixedSize);
}

void KnotMathDialog::recompute() {
	for (const auto &cb : this->callbacks) {
		cb();
	}
}

void KnotMathDialog::closeEvent(QCloseEvent*) {
	Parent->mth = nullptr;
}

}}

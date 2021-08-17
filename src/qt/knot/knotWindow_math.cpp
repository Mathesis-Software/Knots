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

#include "../../math/computables/computables.h"
#include "../../math/computables/experimental.h"
#include "knotWindow_math.h"
#include "KnotWidget.h"

void knotWindow::math() {
	if (mth) {
		mth->show();
		mth->raise();
		return;
	}

	mth = new paramWindow (this);
	mth->setWindowTitle ((QString) "Parameters for " + windowTitle ());
	mth->show ();
}

paramWindow::paramWindow (knotWindow *p) {
	Parent = p;

	const auto &knot = p->knotWidget()->knot();
	std::vector<std::shared_ptr<KE::ThreeD::Computables::Computable>> computables = {
		std::make_shared<KE::ThreeD::Computables::MoebiusEnergy>(knot),
		std::make_shared<KE::ThreeD::Computables::AverageCrossingNumber>(knot, false),
		std::make_shared<KE::ThreeD::Computables::AverageCrossingNumber>(knot, true),
		std::make_shared<KE::ThreeD::Computables::AverageExtremumNumber>(knot),
		std::make_shared<KE::ThreeD::Computables::VassilievInvariant>(knot, 2),
		std::make_shared<KE::ThreeD::Computables::VassilievInvariant>(knot, 3),
		std::make_shared<KE::ThreeD::Computables::VassilievInvariant>(knot, 4),
		std::make_shared<KE::ThreeD::Computables::VassilievInvariant>(knot, 5),
		std::make_shared<KE::ThreeD::Computables::Experimental>(knot)
//		std::make_shared<KE::ThreeD::Computables::Singular>(knot),
//		std::make_shared<KE::ThreeD::Computables::Experimental2>(knot, 2, "Experimental 2"),
//		std::make_shared<KE::ThreeD::Computables::Experimental2>(knot, 3, "Experimental 3"),
//		std::make_shared<KE::ThreeD::Computables::Experimental2>(knot, 4, "Experimental 4"),
	};
	nLabels = computables.size();
	pLabels = new parameterLabel*[nLabels];
	int index = 0;
	for (auto computable : computables) {
		pLabels[index] = new parameterLabel(this, computable, 20, 15 + 30 * index);
		index += 1;
	}
	setFixedSize (380, 25 + 30 * nLabels);
}

void paramWindow::recompute() {
	for (int i = 0; i < nLabels; i++) {
		pLabels[i]->renew();
	}
}

paramWindow::~paramWindow() {
	for (int i = 0; i < nLabels; i++) {
		delete pLabels[i];
	}

	delete[] pLabels;
}

void paramWindow::closeEvent(QCloseEvent*) {
	Parent->mth = NULL;
	delete this;
}

parameterLabel::parameterLabel(QDialog* parent, std::shared_ptr<KE::ThreeD::Computables::Computable> computable, int x, int y) : QWidget(parent), computable(computable) {
	lbl = new QLabel (this);
	lbl->setGeometry (240, 0, 100, 25);
	lbl->setFrameStyle(QFrame::Panel | QFrame::Sunken);

	chkbox = new QCheckBox (computable->name.c_str(), this);
	chkbox->setGeometry (0, 0, 220, 25);
	connect (chkbox, SIGNAL (clicked ()), SLOT (doit ()));

	setGeometry (x, y, 340, 25);
}

parameterLabel::~parameterLabel() {
	delete lbl;
	delete chkbox;
}

void parameterLabel::doit() {
	if (chkbox->isChecked()) {
		lbl->setText(QString::number(computable->value()));
	} else {
		lbl->setText(QString());
	}
}

void parameterLabel::renew() {
	if (chkbox->isChecked()) {
		lbl->setText(QString::number(computable->value()));
	}
}

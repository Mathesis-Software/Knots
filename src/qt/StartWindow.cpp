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

#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QPushButton>

#include "StartWindow.h"

namespace KE::Qt {

StartWindow::StartWindow() {
	this->setCentralWidget(new QWidget);
	auto layout = new QHBoxLayout(this->centralWidget());
	layout->addWidget(this->createButton("Create new diagram", &StartWindow::newDiagram));
	layout->addWidget(this->createButton("Open existing file", [] { return StartWindow::openFile(); }));

	setWindowTitle("Knot Editor");
	this->resize(508, 594);

	this->createFileMenu();
}

QPushButton *StartWindow::createButton(const QString &title, std::function<QWidget*()> creator) {
	auto button = new QPushButton(title);
	button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	button->connect(button, &QPushButton::clicked, [this,creator] {
		auto window = creator();
		if (window) {
			window->setGeometry(this->geometry());
			this->close();
		}
	});
	return button;
}

}

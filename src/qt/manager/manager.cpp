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

#include <QtWidgets/QGridLayout>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>

#include "manager.h"
#include "about.h"
#include "../window/abstractWindow.h"

namespace KE { namespace Qt {

ManagerWindow::ManagerWindow() {
	fileMenu = this->menuBar()->addMenu("File");
	fileMenu->addAction("About", [] { AboutWindow::showAboutDialog(); });
	fileMenu->addSeparator();
	auto quit = fileMenu->addAction("Quit", [] { abstractWindow::exitApplication(); });
	quit->setShortcut(QKeySequence("Ctrl+Q"));

	auto center = new QWidget;
	auto layout = new QGridLayout(center);
	{
		auto button = new QPushButton("Create new diagram");
		button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		button->connect(button, &QPushButton::clicked, [this] {
			auto window = abstractWindow::newDiagram();
			window->setGeometry(this->geometry());
			this->close();
		});
		layout->addWidget(button, 0, 0);
	}
	{
		auto button = new QPushButton("Open existing file");
		button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		button->connect(button, &QPushButton::clicked, [this] {
			auto window = abstractWindow::openFile();
			if (window) {
				window->setGeometry(this->geometry());
				this->close();
			}
		});
		layout->addWidget(button, 0, 1);
	}
	this->setCentralWidget(center);

	setWindowTitle("Knot Editor");
	this->resize(508, 594);
}

ManagerWindow::~ManagerWindow() {
	delete fileMenu;
}

}}

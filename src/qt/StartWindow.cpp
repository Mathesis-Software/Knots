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

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>

#include "KnotEditorApplication.h"
#include "StartWindow.h"
#include "Window.h"

namespace KE::Qt {

namespace {

QPushButton *createButton(StartWindow *start, const QString &title, std::function<QWidget*()> creator) {
	auto button = new QPushButton(title);
	button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	button->connect(button, &QPushButton::clicked, [start, creator] {
		auto widget = creator();
		if (widget) {
			auto window = dynamic_cast<Window*>(widget);
			if (window) {
				window->setGeometry(start->geometry());
			}
		}
	});
	return button;
}

}

StartWindow::StartWindow() {
	this->setCentralWidget(new QWidget);
	auto layout = new QVBoxLayout(this->centralWidget());
	layout->addWidget(createButton(this, "Create new diagram", [] { return dynamic_cast<KnotEditorApplication*>(qApp)->newDiagram(); }));
	layout->addWidget(createButton(this, "Browse the library", [] { return dynamic_cast<KnotEditorApplication*>(qApp)->library(); }));
	layout->addWidget(createButton(this, "Open your own file", [] { return dynamic_cast<KnotEditorApplication*>(qApp)->openFile(); }));

	setWindowTitle("Knot Editor");
	this->resize(508, 594);
}

}

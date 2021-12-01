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

#include <QtCore/QFile>
#include <QtGui/QShortcut>
#include <QtGui/QScreen>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>

#include "AboutDialog.h"
#include "Application.h"
#include "Version.h"

namespace KE::Qt {

namespace {

AboutDialog *instance = nullptr;

}

void AboutDialog::showAboutDialog() {
	// TODO: add mutex
	if (instance) {
		instance->show();
		instance->raise();
	} else {
		instance = new AboutDialog();
		instance->show();
		auto screen = instance->screen()->virtualSize();
		auto current = instance->size();
		instance->move((screen.width() - current.width()) / 2, (screen.height() - current.height()) / 2);
	}
}

AboutDialog::AboutDialog() {
	//this->setWindowFlags(::Qt::FramelessWindowHint | ::Qt::WindowStaysOnTopHint);
	this->setAttribute(::Qt::WA_DeleteOnClose);
	//this->setModal(true);
	this->setObjectName("about");
	this->setWindowTitle(tr("About Knots"));

	new QShortcut(QKeySequence("Ctrl+W"), this, this, &AboutDialog::close);
	new QShortcut(QKeySequence("Ctrl+Q"), this, static_cast<Application*>(qApp), &Application::quitApplication);

	auto layout = new QHBoxLayout(this);
	layout->setContentsMargins(0, 0, 0, 0);
	layout->setSpacing(0);

	auto logo = new QLabel(this);
	logo->setObjectName("logo");
	QPixmap pixmap(":images/trefoil.png");
	pixmap.setDevicePixelRatio(this->devicePixelRatio());
	logo->setPixmap(pixmap.scaled(256, 256, ::Qt::IgnoreAspectRatio, ::Qt::SmoothTransformation));
	layout->addWidget(logo);

	auto text = new QLabel(this);
	text->setObjectName("text");

	QFile file(":html/about.html");
	file.open(QFile::ReadOnly);
	QString content(file.readAll());
	content.replace("${version}", QString::fromStdString(Version::version));
	content.replace("${build}", QString::fromStdString(Version::build()));
	text->setText(content);

	text->setTextFormat(::Qt::RichText);
	text->setTextInteractionFlags(::Qt::TextBrowserInteraction);
	text->setOpenExternalLinks(true);
	layout->addWidget(text);
}

AboutDialog::~AboutDialog() {
	instance = nullptr;
}

}

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

#include <QtGui/QAction>
#include <QtGui/QScreen>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

#include "AboutWindow.h"
#include "Application.h"

namespace KE::Qt {

void AboutWindow::showAboutDialog() {
	auto about = new AboutWindow();
	const QRect screenGeometry = about->screen()->geometry();
	const int x = (screenGeometry.width() - 380) / 2;
	const int y = (screenGeometry.height() - 180) / 2;
	about->move(x, y);
	about->show();
}

AboutWindow::AboutWindow() {
	this->setWindowFlags(::Qt::Window | ::Qt::FramelessWindowHint | ::Qt::WindowStaysOnTopHint);
	this->setAttribute(::Qt::WA_DeleteOnClose);
	this->setWindowModality(::Qt::ApplicationModal);
	setFixedSize(380, 200);
	QPalette pal = this->palette();
	pal.setColor(QPalette::Window, ::Qt::white);
	this->setAutoFillBackground(true);
	this->setPalette(pal);

	auto icon = new QLabel(this);
	icon->setGeometry(36, 36, 128, 128);
	QPixmap pixmap(":images/trefoil.png");
	const auto dpr = this->devicePixelRatio();
	pixmap.setDevicePixelRatio(dpr);
	icon->setPixmap(pixmap.scaled(128 * dpr, 128 * dpr, ::Qt::IgnoreAspectRatio, ::Qt::SmoothTransformation));

	auto title = new QLabel(QString("Knot Editor<br/>") + VERSION, this);
	title->setGeometry(180, 10, 150, 120);
	title->setFont(QFont("Helvetica", 14));
	title->setAlignment(::Qt::AlignCenter);
	auto link = new QLabel("<a href='https://knots.geometer.name/'>knots.geometer.name</a>", this);
	link->setGeometry(180, 60, 150, 120);
	link->setFont(QFont("Helvetica", 10));
	link->setAlignment(::Qt::AlignCenter);
	link->setOpenExternalLinks(true);

	auto button = new QPushButton("Ok", this);
	button->move(280, 160);
	button->setFocus();
	button->setDefault(true);
	QObject::connect(button, &QPushButton::clicked, [this] { this->close(); });

	auto close = new QAction();
	close->setShortcut(QKeySequence("Ctrl+W"));
	QObject::connect(close, &QAction::triggered, [this] { this->close(); });
	this->addAction(close);

	auto quit = new QAction();
	quit->setShortcut(QKeySequence("Ctrl+Q"));
	QObject::connect(quit, &QAction::triggered, [this] {
		this->close();
		dynamic_cast<Application*>(qApp)->exitApplication();
	});
	this->addAction(quit);
}

}

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

#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDesktopWidget>
#include <QtWidgets/QLabel>

#include "AboutWindow.h"
#include "Window.h"

namespace KE::Qt {

void AboutWindow::showAboutDialog() {
	(new AboutWindow())->showMe();
}

AboutWindow::AboutWindow() {
	this->setWindowFlags(::Qt::Window | ::Qt::FramelessWindowHint | ::Qt::WindowStaysOnTopHint);
	this->setAttribute(::Qt::WA_DeleteOnClose);
	this->setWindowModality(::Qt::ApplicationModal);
	setFixedSize(380, 180);
	QPalette pal = this->palette();
	pal.setColor(QPalette::Background, ::Qt::white);
	this->setAutoFillBackground(true);
	this->setPalette(pal);

	auto icon = new QLabel(this);
	icon->setGeometry(36, 26, 128, 128);
	QPixmap pixmap(":images/trefoil.png");
	const auto dpr = this->devicePixelRatio();
	pixmap.setDevicePixelRatio(dpr);
	icon->setPixmap(pixmap.scaled(128 * dpr, 128 * dpr, ::Qt::IgnoreAspectRatio, ::Qt::SmoothTransformation));

	auto text = new QLabel(this);
	text->setGeometry(180, 30, 150, 120);
	QFont fnt("Helvetica", 14);
	text->setFont(fnt);
	text->setAlignment(::Qt::AlignCenter);
	text->setText(QString("Knot Editor\nversion ") + VERSION);

	auto close = new QAction();
	close->setShortcut(QKeySequence("Ctrl+W"));
	QObject::connect(close, &QAction::triggered, [this] { this->close(); });
	this->addAction(close);

	auto quit = new QAction();
	quit->setShortcut(QKeySequence("Ctrl+Q"));
	QObject::connect(quit, &QAction::triggered, [this] {
		this->close();
		Window::exitApplication();
	});
	this->addAction(quit);
}

void AboutWindow::showMe() {
	const QRect screenGeometry = QApplication::desktop()->screenGeometry();
	const int x = (screenGeometry.width() - 380) / 2;
	const int y = (screenGeometry.height() - 180) / 2;
	this->move(x, y);
	this->show();
}

void AboutWindow::mousePressEvent(QMouseEvent*) {
	this->close();
}

}

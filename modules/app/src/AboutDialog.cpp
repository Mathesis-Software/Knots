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

#include <QtGui/QAction>
#include <QtGui/QScreen>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

#include "AboutDialog.h"
#include "Application.h"

namespace KE::Qt {

void AboutDialog::showAboutDialog() {
	auto about = new AboutDialog();
	about->show();
	const QRect screenGeometry = about->screen()->geometry();
	const int x = (screenGeometry.width() - about->width()) / 2;
	const int y = (screenGeometry.height() - about->height()) / 2;
	about->move(x, y);
}

AboutDialog::AboutDialog() {
	this->setWindowFlags(::Qt::FramelessWindowHint | ::Qt::WindowStaysOnTopHint);
	this->setAttribute(::Qt::WA_DeleteOnClose);
	this->setModal(true);
	QPalette pal = this->palette();
	pal.setColor(QPalette::Window, ::Qt::white);
	this->setAutoFillBackground(true);
	this->setPalette(pal);

	auto main = new QVBoxLayout(this);
	main->setSpacing(5);
	auto hlayout = new QHBoxLayout;
	hlayout->setContentsMargins(10, 5, 30, 0);
	hlayout->setSpacing(15);
	main->addLayout(hlayout);
	QPixmap pixmap(":images/trefoil.png");
	const auto dpr = this->devicePixelRatio();
	pixmap.setDevicePixelRatio(dpr);
	auto icon = new QLabel;
	hlayout->addWidget(icon);
	auto vlayout = new QVBoxLayout;
	vlayout->setContentsMargins(0, 10, 0, 0);
	vlayout->setSpacing(10);
	hlayout->addLayout(vlayout);

	auto title = new QLabel(QString("Mathesis Knots<br/>") + VERSION, this);
	auto font = title->font();
	font.setPointSize(font.pointSize() * 1.5);
	title->setFont(font);
	title->setAlignment(::Qt::AlignHCenter);
	vlayout->addWidget(title);
	auto link = new QLabel("<a href='https://knots.mathesis.fun/'>knots.mathesis.fun</a>", this);
	link->setAlignment(::Qt::AlignHCenter);
	link->setOpenExternalLinks(true);
	vlayout->addWidget(link);

	const int height = vlayout->sizeHint().height() * 1.2 * dpr;
	icon->setPixmap(pixmap.scaled(height, height, ::Qt::IgnoreAspectRatio, ::Qt::SmoothTransformation));

	auto box = new QDialogButtonBox(QDialogButtonBox::Ok, this);
	QObject::connect(box, &QDialogButtonBox::accepted, this, &QDialog::accept);
	main->addWidget(box);
	main->setSizeConstraint(QLayout::SetFixedSize);

	auto close = new QAction();
	close->setShortcut(QKeySequence("Ctrl+W"));
	QObject::connect(close, &QAction::triggered, [this] { this->close(); });
	this->addAction(close);

	auto quit = new QAction();
	quit->setShortcut(QKeySequence("Ctrl+Q"));
	QObject::connect(quit, &QAction::triggered, [this] {
		this->close();
		dynamic_cast<Application *>(qApp)->exitApplication();
	});
	this->addAction(quit);
}

}

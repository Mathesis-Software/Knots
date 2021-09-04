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

#include <QtCore/QSettings>
#include <QtGui/QPainter>
#include <QtWidgets/QApplication>
#include <QtWidgets/QProxyStyle>

#include "StartWindow.h"

namespace {

class ProxyStyle : public QProxyStyle {

QPixmap generatedIconPixmap(QIcon::Mode iconMode, const QPixmap &pixmap, const QStyleOption *option) const override {
	if (iconMode == QIcon::Disabled) {
		QPixmap copy(pixmap);
		QPainter painter(&copy);
		painter.setCompositionMode(QPainter::CompositionMode_SourceAtop);
		painter.fillRect(copy.rect(), 0xc0c0c0);
		painter.end();
		return copy;
	}
	return QProxyStyle::generatedIconPixmap(iconMode, pixmap, option);
}

};

}

int main(int argc, char **argv) {
	QApplication::setOrganizationName("Knot Editor");
	QApplication::setApplicationName("Knot Editor");
	QApplication::setAttribute(Qt::AA_DontUseNativeDialogs);
	QApplication qa(argc, argv);
	qa.setFont(QFont("Helvetica", 10));
	qa.setStyle(new ProxyStyle);

	QPixmap pixmap(":images/trefoil.png");
	pixmap.setDevicePixelRatio(qa.devicePixelRatio());
	qa.setWindowIcon(pixmap);

	int count = 0;
	for (int i = 1; i < argc; ++i) {
		if (auto window = KE::Qt::StartWindow::openFile(argv[i])) {
			window->raise();
			count += 1;
		}
	}
	if (count == 0) {
		QSettings settings;
		for (const auto &name : settings.value("OpenWindows").toStringList()) {
			settings.beginGroup("Window:" + name);
			auto geometry = settings.value("geometry").toRect();
			settings.endGroup();
			if (name == "::LIBRARY::") {
				auto window = KE::Qt::StartWindow::library();
				if (geometry.isValid()) {
					window->setGeometry(geometry);
				}
				window->raise();
				count += 1;
			} else if (auto window = KE::Qt::StartWindow::openFile(name)) {
				if (geometry.isValid()) {
					window->setGeometry(geometry);
				}
				window->raise();
				count += 1;
			}
		}
	}
	if (count == 0) {
		(new KE::Qt::StartWindow())->show();
	}

	return qa.exec();
}

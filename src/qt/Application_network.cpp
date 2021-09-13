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

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtWidgets/QMessageBox>

#include "Application.h"
#include "DiagramWindow.h"
#include "../ke/Util_rapidjson.h"

namespace KE::Qt {

void Application::diagramFromCode(const QString &code) {
	QUrl url("https://knots.geometer.name/api/diagram4code");
	QNetworkRequest request;
	request.setUrl(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	QObject::connect(manager, &QNetworkAccessManager::finished, this, [this, code](QNetworkReply *reply) {
		try {
			rapidjson::Document doc;
			const auto data = reply->readAll();
			doc.Parse(data.constData(), data.size());
			if (doc.IsObject() && Util::rapidjson::getString(doc, "type") == "diagram") {
				auto window = new DiagramWindow(doc, QString());
				window->show();
				this->closeStartWindow();
			} else if (doc.IsObject() && Util::rapidjson::getString(doc, "error") != "") {
				throw std::runtime_error(Util::rapidjson::getString(doc, "error"));
			} else {
				throw std::runtime_error(QString(data).toStdString());
			}
		} catch (const std::runtime_error &e) {
			QMessageBox::critical(nullptr, "Error for " + code, QString("\n") + e.what() + "\n");
		}
	});
	QJsonObject data;
	data["code"] = code;
	data["debug"] = true;
	manager->post(request, QJsonDocument(data).toJson());
}

}

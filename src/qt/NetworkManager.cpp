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
#include <QtNetwork/QNetworkReply>

#include "NetworkManager.h"

namespace KE::Qt {

// TODO: add network error callback
void NetworkManager::searchDiagram(const QString &code, const std::function<void(const QByteArray &response)> callback) {
	QUrl url("https://knots.geometer.name/api/diagram4code");
	QNetworkRequest request;
	request.setUrl(url);
	request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
	QObject::connect(this, &QNetworkAccessManager::finished, this->parent(), [callback](QNetworkReply *reply) {
		// TODO: veerify request id
		// TODO: disconnect this callback
		callback(reply->readAll());
	});
	QJsonObject data;
	data["code"] = code;
	data["debug"] = true;
	this->post(request, QJsonDocument(data).toJson());
}

}

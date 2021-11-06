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

#include <QtCore/QResource>
#include <QtGui/QMovie>
#include <QtWidgets/QMessageBox>

#include <Util_rapidjson.h>

#include "Application.h"
#include "LibraryModel.h"
#include "LibraryWindow.h"
#include "NetworkManager.h"
#include "Preview.h"

namespace KE::Qt {

void DataItem::init(const rapidjson::Value &doc) {
	if (doc.IsNull()) {
		throw std::runtime_error("The data are not in JSON format");
	}

	const std::string type = KE::Util::rapidjson::getString(doc, "type");
	std::string name;
	QPixmap pixmap(400, 400);
	pixmap.fill(::Qt::white);
	if (type == "link") {
		const ThreeD::Knot knot(doc);
		this->setText(knot.caption.c_str());
		KnotPreview(knot).paint(pixmap);
	} else if (type == "diagram") {
		const TwoD::Diagram diagram(doc);
		this->setText(diagram.caption.c_str());
		DiagramPreview(diagram).paint(pixmap);
	} else {
		throw std::runtime_error("The data do not represent a knot nor a diagram");
	}

	this->setTextAlignment(::Qt::AlignCenter);
	this->setSizeHint(QSize(120, 145));
	QIcon icon;
	icon.addPixmap(pixmap, QIcon::Normal);
	icon.addPixmap(pixmap, QIcon::Selected);
	this->setIcon(icon);
}

FileDataItem::FileDataItem(const QString &path, int index) : path(path), index(index) {
	QResource resource(this->path);
	rapidjson::Document doc;
	doc.Parse(reinterpret_cast<const char*>(resource.data()), resource.size());
	this->init(doc);
}

bool FileDataItem::operator<(const QListWidgetItem &other) const {
	const FileDataItem &data = dynamic_cast<const FileDataItem&>(other);
	return this->index < data.index || (this->index == data.index && this->path < data.path);
}

void FileDataItem::open() const {
	dynamic_cast<Application*>(qApp)->openFile(this->path);
}

namespace {

class WaitingItem : public QListWidgetItem {

private:
	QAbstractItemModel *model;
	const int position;
	QMovie movie;

public:
	WaitingItem(QAbstractItemModel *model, int position) : model(model), position(position), movie(":images/loader.gif") {
		QObject::connect(&this->movie, &QMovie::frameChanged, [this] {
			QPixmap pixmap(this->movie.currentPixmap());
			QIcon icon;
			icon.addPixmap(pixmap, QIcon::Normal);
			icon.addPixmap(pixmap, QIcon::Selected);
			this->setIcon(icon);
			const auto index = this->model->index(this->position, 0);
			emit this->model->dataChanged(index, index);
		});
		if (this->movie.loopCount() != -1) {
			QObject::connect(&this->movie, &QMovie::finished, &this->movie, &QMovie::start);
		}
		this->movie.start();
		this->setText("Loading…");
		this->setSizeHint(QSize(120, 145));
	}
};

class JsonDataItem : public DataItem {

private:
	const std::string json;

public:
	JsonDataItem(const rapidjson::Value &value) : json(Util::rapidjson::docToString(value)) {
		this->init(value);
	}

private:
	void open() const override {
		rapidjson::Document doc;
		doc.Parse(json.data(), json.size());
		dynamic_cast<Application*>(qApp)->openDocument(doc, QString());
	}
};

}

LibraryModel::LibraryModel(const QList<QListWidgetItem *> &items) : items(items) {
}

LibraryModel::~LibraryModel() {
	this->clear();
}

const DataItem *LibraryModel::dataItem(const QModelIndex &index) const {
	if (!index.isValid()) {
		return nullptr;
	}
	return dynamic_cast<DataItem*>(this->items.at(index.row()));
}

void LibraryModel::addItems(QList<QListWidgetItem*> items) {
	this->beginInsertRows(QModelIndex(), this->items.count(), this->items.count() + items.count() - 1);
	this->items.append(items);
	this->endInsertRows();
}

void LibraryModel::addItem(QListWidgetItem *item) {
	this->beginInsertRows(QModelIndex(), this->items.count(), this->items.count());
	this->items.append(item);
	this->endInsertRows();
}

void LibraryModel::removeItem(QListWidgetItem *item) {
	if (item == nullptr) {
		return;
	}

	const auto position = this->items.indexOf(item);
	if (position != -1) {
		this->beginRemoveRows(QModelIndex(), position, position);
		this->items.removeAt(position);
		this->endRemoveRows();
	}
	delete item;
}

void LibraryModel::clear() {
	this->beginResetModel();
	for (int i = 0; i < this->items.count(); ++i) {
		if (this->items.at(i)) {
			delete this->items.at(i);
		}
	}
	this->items.clear();
	this->endResetModel();
}

int LibraryModel::rowCount(const QModelIndex &parent) const {
	return parent.isValid() ? 0 : this->items.count();
}

QVariant LibraryModel::data(const QModelIndex &index, int role) const {
	if (!index.isValid() || index.row() >= this->items.count()) {
		return QVariant();
	}
	return this->items.at(index.row())->data(role);
}

NetworkLibraryModel::NetworkLibraryModel(LibraryWindow *window, const QString &pattern) : LibraryModel(QList<QListWidgetItem*>()), window(window), pattern(pattern), nextPage(0) {
}

bool NetworkLibraryModel::canFetchMore(const QModelIndex &index) const {
	return this->nextPage >= 0;
}

void NetworkLibraryModel::fetchMore(const QModelIndex &index) {
	const auto page = this->nextPage;
	this->nextPage = -1;

	auto waitingItem = new WaitingItem(this, this->rowCount());
	this->addItem(waitingItem);
	this->window->networkManager()->searchDiagram(pattern, page, this, [=](int errorCode, const QByteArray &data) {
		try {
			this->removeItem(waitingItem);
			if (errorCode != 0) {
				throw std::runtime_error("Network error");
			}
			rapidjson::Document document;
			document.Parse(reinterpret_cast<const char*>(data.data()), data.size());
			if (!document.IsObject()) {
				throw std::runtime_error("Invalid response format");
			}
			const std::string error = KE::Util::rapidjson::getString(document, "error");
			if (!error.empty()) {
				throw std::runtime_error(error.c_str());
			}
			if (document.HasMember("layouts") && document["layouts"].IsArray()) {
				const auto &layouts = document["layouts"];
				QList<QListWidgetItem*> items;
				for (std::size_t i = 0; i < layouts.Size(); i += 1) {
					items.append(new JsonDataItem(layouts[i]));
				}
				this->addItems(items);
			} else {
				throw std::runtime_error("No layouts in the response");
			}
			if (document.HasMember("hasNextPage")) {
				const auto &hnp = document["hasNextPage"];
				if (hnp.IsBool() && hnp.GetBool()) {
					this->nextPage = page + 1;
				}
			}
		} catch (const std::runtime_error &e) {
			// TODO: better way to report error
			QMessageBox::warning(this->window, "Error", QString("\n") + e.what() + "\n");
		}
	});
}

}

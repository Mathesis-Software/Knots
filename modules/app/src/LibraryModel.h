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

#ifndef __KE_QT_LIBRARY_MODEL_H__
#define __KE_QT_LIBRARY_MODEL_H__

#include <rapidjson/document.h>

#include <QtCore/QAbstractListModel>
#include <QtWidgets/QListWidgetItem>

namespace KE::Qt {

class LibraryWindow;

class DataItem : public QListWidgetItem {

public:
	virtual void open() const = 0;

protected:
	void init(const rapidjson::Value &doc);
};

class FileDataItem : public DataItem {

private:
	const QString path;
	const int index;

public:
	FileDataItem(const QString &path, int index);

	bool operator<(const QListWidgetItem &other) const override;

private:
	void open() const override;
};

class LibraryModel : public QAbstractListModel {

private:
	QList<QListWidgetItem *> items;

public:
	LibraryModel(const QList<QListWidgetItem *> &items);
	~LibraryModel();

	const DataItem *dataItem(const QModelIndex &index) const;

	void addItems(QList<QListWidgetItem*> items);
	void addItem(QListWidgetItem *item);
	void removeItem(QListWidgetItem *item);
	void clear();

protected:
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;

private:
	QVariant data(const QModelIndex &index, int role) const override;
};

class NetworkLibraryModel : public LibraryModel {

private:
	LibraryWindow *window;
	const QString pattern;
	volatile int nextPage;

public:
	NetworkLibraryModel(LibraryWindow *window, const QString &pattern);

private:
	bool canFetchMore(const QModelIndex&) const override;
	void fetchMore(const QModelIndex&) override;
};

}

#endif /* __KE_QT_LIBRARY_MODEL_H__ */

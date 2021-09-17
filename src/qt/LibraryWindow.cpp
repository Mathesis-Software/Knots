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

#include <cmath>

#include <rapidjson/document.h>

#include <QtCore/QDirIterator>
#include <QtCore/QResource>
#include <QtCore/QSettings>
#include <QtCore/QStringListModel>
#include <QtGui/QAction>
#include <QtGui/QDesktopServices>
#include <QtGui/QMovie>
#include <QtGui/QKeyEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QCompleter>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QVBoxLayout>

#include "Application.h"
#include "LibraryWindow.h"
#include "NetworkManager.h"
#include "../ke/Diagram.h"
#include "../ke/Knot.h"
#include "../ke/Util_rapidjson.h"

namespace KE::Qt {

namespace {

class KnotPreview {

private:
	const ThreeD::Knot &knot;

public:
	KnotPreview(const ThreeD::Knot &knot) : knot(knot) {
	}

	void paint(QPixmap &pixmap) {
		QPainter painter;
		painter.begin(&pixmap);

		double minX = std::numeric_limits<double>::max();
		double minY = std::numeric_limits<double>::max();
		double maxX = std::numeric_limits<double>::min();
		double maxY = std::numeric_limits<double>::min();
		const auto snapshot = this->knot.snapshot();
		for (std::size_t i = 0; i < snapshot.size(); i += 1) {
			const auto &point = snapshot[i];
			minX = std::min(minX, point.x);
			minY = std::min(minY, point.y);
			maxX = std::max(maxX, point.x);
			maxY = std::max(maxY, point.y);
		}

		const double scale = 0.8 * pixmap.width() / std::max(maxX - minX, maxY - minY);
		const double deltaX = (pixmap.width() - scale * (minX + maxX)) / 2;
		const double deltaY = (pixmap.height() - scale * (minY + maxY)) / 2;

		typedef std::pair<ThreeD::Point,ThreeD::Point> Pair;
		std::vector<Pair> edges;
		for (std::size_t i = 0; i < snapshot.size(); i += 1) {
			edges.push_back(std::make_pair(snapshot[i], snapshot[snapshot.next(i)]));
		}
		std::sort(edges.begin(), edges.end(), [](const Pair &p0, const Pair &p1){ return p0.first.z < p1.first.z; });
		for (const auto &pair : edges) {
			QPointF start(pair.first.x * scale + deltaX, pixmap.height() - pair.first.y * scale - deltaY);
			QPointF end(pair.second.x * scale + deltaX, pixmap.height() / pixmap.devicePixelRatio() - pair.second.y * scale - deltaY);
			painter.setPen(QPen(::Qt::white, 24, ::Qt::SolidLine, ::Qt::FlatCap));
			painter.drawLine(start, end);
			painter.setPen(QPen(::Qt::gray, 8, ::Qt::SolidLine, ::Qt::FlatCap));
			painter.drawLine(start, end);
		}

		painter.end();
	}
};

class DiagramPreview {

private:
	const TwoD::Diagram &diagram;

public:
	DiagramPreview(const TwoD::Diagram &diagram) : diagram(diagram) {
	}

	void paint(QPixmap &pixmap) {
		QPainter painter;
		painter.begin(&pixmap);

		float minX = std::numeric_limits<float>::max();
		float minY = std::numeric_limits<float>::max();
		float maxX = std::numeric_limits<float>::min();
		float maxY = std::numeric_limits<float>::min();
		for (const auto &vertex : this->diagram.vertices()) {
			const auto coords = vertex->coords();
			minX = std::min(minX, coords.x);
			minY = std::min(minY, coords.y);
			maxX = std::max(maxX, coords.x);
			maxY = std::max(maxY, coords.y);
		}

		const float scale = 0.8 * pixmap.width() / pixmap.devicePixelRatio() / std::max(maxX - minX, maxY - minY);
		const float deltaX = (pixmap.width() / pixmap.devicePixelRatio() - scale * (minX + maxX)) / 2;
		const float deltaY = (pixmap.width() / pixmap.devicePixelRatio() - scale * (minY + maxY)) / 2;

		painter.setPen(QPen(::Qt::black, 4 * scale));
		painter.setBrush(::Qt::black);

		for (const auto &edge : this->diagram.edges()) {
			drawEdge(painter, edge, scale, deltaX, deltaY);
		}
		const auto radius = 6 * scale;
		for (const auto &vertex : this->diagram.vertices()) {
			const auto coords = vertex->coords();
			const QPointF center(scale * coords.x + deltaX, scale * coords.y + deltaY);
			painter.drawEllipse(center, radius, radius);
		}

		painter.end();
	}

private:
	void drawEdge(QPainter &painter, const TwoD::Diagram::Edge &edge, float scale, float dX, float dY) {
		const auto start = edge.start->coords();
		const auto end = edge.end->coords();
		float deltaX = end.x - start.x;
		float deltaY = end.y - start.y;
		float hyp = hypotf(deltaX, deltaY);

		deltaX = 20 * deltaX / hyp;
		deltaY = 20 * deltaY / hyp;

		float x0 = start.x,
					y0 = start.y,
					x1, y1;

		for (const auto &crs : this->diagram.underCrossings(edge)) {
			auto coords = crs.coords();
			if (!coords) {
				continue;
			}
			x1 = coords->x - deltaX;
			y1 = coords->y - deltaY;

			if ((x1 - x0) * deltaX + (y1 - y0) * deltaY > 0) {
				painter.drawLine(QPointF(x0 * scale + dX, y0 * scale + dY), QPointF(x1 * scale + dX, y1 * scale + dY));
			}

			x0 = coords->x + deltaX;
			y0 = coords->y + deltaY;
		}

		x1 = end.x;
		y1 = end.y;

		if ((x1 - x0) * deltaX + (y1 - y0) * deltaY > 0) {
			painter.drawLine(QPointF(x0 * scale + dX, y0 * scale + dY), QPointF(x1 * scale + dX, y1 * scale + dY));
		}
	}
};

class DataItem : public QListWidgetItem {

public:
	virtual void open() const = 0;

protected:
	void init(const rapidjson::Value &doc) {
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
};

class FileDataItem : public DataItem {

private:
	const QString path;
	const int index;

public:
	FileDataItem(const QString &path, int index) : path(path), index(index) {
		QResource resource(this->path);
		rapidjson::Document doc;
		doc.Parse(reinterpret_cast<const char*>(resource.data()), resource.size());
		this->init(doc);
	}

	bool operator < (const QListWidgetItem &other) const override {
		const FileDataItem &data = dynamic_cast<const FileDataItem&>(other);
		return this->index < data.index || (this->index == data.index && this->path < data.path);
	}

private:
	void open() const override {
		dynamic_cast<Application*>(qApp)->openFile(this->path);
	}
};

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

class LibraryModel : public QAbstractListModel {

private:
	QList<QListWidgetItem*> items;

public:
	LibraryModel(const QList<QListWidgetItem*> &items) : items(items) {
	}

	~LibraryModel() {
		this->clear();
	}

	const DataItem *dataItem(const QModelIndex &index) const {
		if (!index.isValid()) {
			return nullptr;
		}
		return dynamic_cast<DataItem*>(this->items.at(index.row()));
	}

	void addItems(QList<QListWidgetItem*> items) {
		this->beginInsertRows(QModelIndex(), this->items.count(), this->items.count() + items.count() - 1);
		this->items.append(items);
		this->endInsertRows();
	}

	void addItem(QListWidgetItem* item) {
		this->beginInsertRows(QModelIndex(), this->items.count(), this->items.count());
		this->items.append(item);
		this->endInsertRows();
	}

	void removeItem(QListWidgetItem *item) {
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

	void clear() {
		this->beginResetModel();
		for (int i = 0; i < this->items.count(); ++i) {
			if (this->items.at(i)) {
				delete this->items.at(i);
			}
		}
		this->items.clear();
		this->endResetModel();
	}

protected:
	int rowCount(const QModelIndex &parent = QModelIndex()) const override {
		return parent.isValid() ? 0 : this->items.count();
	}

private:
	QVariant data(const QModelIndex &index, int role) const override {
		if (!index.isValid() || index.row() >= this->items.count()) {
			return QVariant();
		}
		return this->items.at(index.row())->data(role);
	}
};

class LibraryListView : public QListView {

public:
	LibraryListView(const QList<QListWidgetItem*> &items = QList<QListWidgetItem*>()) {
		this->setModel(new LibraryModel(items));
		this->setMouseTracking(true);
		this->setViewMode(QListView::IconMode);
		this->setResizeMode(QListView::Adjust);
		this->setContentsMargins(0, 0, 0, 0);
		this->setIconSize(QSize(100, 100));
		this->setSpacing(5);
		this->setUniformItemSizes(true);
		this->setStyleSheet("QListView{background:#d8d8d8;} QListView::item{background:white;border:1px solid #c0c0c0;color:#808080;} QListView::item::selected{border:2px solid #404040;}");
		this->setSelectionMode(QListView::SingleSelection);

		QObject::connect(this, &QListWidget::entered, [this](const QModelIndex &index) {
			this->selectionModel()->setCurrentIndex(index, QItemSelectionModel::ClearAndSelect);
		});
		QObject::connect(this, &QListWidget::clicked, [this](const QModelIndex &index) {
			if (auto item = static_cast<const LibraryModel*>(this->model())->dataItem(index)) {
				item->open();
			}
		});
		QObject::connect(this->selectionModel(), &QItemSelectionModel::currentChanged, [this](const QModelIndex &current, const QModelIndex &/*previous*/) {
			if (current.isValid() && !this->selectionModel()->isSelected(current)) {
				this->selectionModel()->setCurrentIndex(QModelIndex(), QItemSelectionModel::ClearAndSelect);
			}
		});
	}

	~LibraryListView() {
		this->model()->deleteLater();
	}

	void setModel(QAbstractItemModel *model) override {
		auto sele = this->selectionModel();
		if (sele) {
			sele->clear();
		}
		auto old = this->model();
		QListView::setModel(model);
		if (old) {
			old->deleteLater();
		}
	}

private:
	void leaveEvent(QEvent*) override {
		this->selectionModel()->setCurrentIndex(QModelIndex(), QItemSelectionModel::ClearAndSelect);
	}

	void keyPressEvent(QKeyEvent *event) override {
		if (event->key() != ::Qt::Key_Enter && event->key() != ::Qt::Key_Return) {
			QListView::keyPressEvent(event);
			return;
		}

		if (auto item = static_cast<const LibraryModel*>(this->model())->dataItem(this->currentIndex())) {
			item->open();
		}
	}
};

class NetworkLibraryModel : public LibraryModel {

private:
	LibraryWindow *window;
	const QString pattern;
	volatile int nextPage;

public:
	NetworkLibraryModel(LibraryWindow *window, const QString &pattern) : LibraryModel(QList<QListWidgetItem*>()), window(window), pattern(pattern), nextPage(0) {}

private:
	bool canFetchMore(const QModelIndex&) const override {
		return this->nextPage >= 0;
	}

	void fetchMore(const QModelIndex&) override {
		const auto page = this->nextPage;
		this->nextPage = -1;

		auto waitingItem = new WaitingItem(this, this->rowCount());
		this->addItem(waitingItem);
		this->window->networkManager()->searchDiagram(pattern, page, [=] (int errorCode, const QByteArray &data) {
			try {
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
					this->removeItem(waitingItem);
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
				this->removeItem(waitingItem);
			}
		});
	}
};

class SearchBar : public QLineEdit {

private:
	QStringListModel *historyModel;

public:
	SearchBar() {
		const QSettings settings;
		const QStringList history = settings.value("NetworkSearchHistory").toStringList();
		this->historyModel = new QStringListModel(history, this);
		this->setCompleter(new QCompleter(this->historyModel, this));

		this->addAction(QIcon(":images/search.svg"), QLineEdit::LeadingPosition);
		auto helpAction = this->addAction(QIcon(":images/question.svg"), QLineEdit::TrailingPosition);
		QObject::connect(helpAction, &QAction::triggered, [=] {
			QDesktopServices::openUrl(QUrl("https://knots.geometer.name/help/search"));
		});
	}

	void contextMenuEvent(QContextMenuEvent *event) override {
    QMenu *menu = createStandardContextMenu();
    menu->addSeparator();
    auto cleaning = menu->addAction(QIcon(":images/cleaning.svg"), "Clear History");
		QObject::connect(cleaning, &QAction::triggered, [this] {
			this->historyModel->setStringList(QStringList());
			this->saveHistory();
		});
		cleaning->setEnabled(!this->historyModel->stringList().isEmpty());
    menu->exec(event->globalPos());
    delete menu;
	}

	void addPattern(const QString &pattern) {
		if (pattern.isEmpty()) {
			return;
		}

		const auto index = this->historyModel->stringList().indexOf(pattern);
		if (index > 0) {
			this->historyModel->removeRows(index, 1);
		}
		if (index != 0) {
			this->historyModel->insertRows(0, 1);
			this->historyModel->setData(this->historyModel->index(0), pattern);
			this->saveHistory();
		}
	}

private:
	void saveHistory() {
		QSettings settings;
		settings.setValue("NetworkSearchHistory", this->historyModel->stringList().mid(0, 100));
		settings.sync();
	}
};

}

LibraryWindow::LibraryWindow() : _networkManager(new NetworkManager(this)) {
	this->setCentralWidget(new QWidget);
	auto vlayout = new QVBoxLayout(this->centralWidget());
	vlayout->setSpacing(0);
	vlayout->setContentsMargins(0, 0, 0, 0);
	auto top = new QHBoxLayout;
	top->setContentsMargins(0, 6, 0, 0);
	vlayout->addLayout(top);

	auto tabs = new QTabBar;
	tabs->setDocumentMode(true);
	auto diagrams = createList(".dgr");
	auto knots = createList(".knt");
	auto searchResults = new LibraryListView();
	tabs->addTab("Diagrams");
	tabs->addTab("Knots");
	const auto fakeTabIndex = tabs->addTab(QString());
	tabs->setTabVisible(fakeTabIndex, false);
	top->addWidget(tabs);
	top->addItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum));
	auto searchLine = new SearchBar;
	searchLine->setMinimumHeight(24);
	searchLine->setMaximumWidth(200);
	searchLine->setAttribute(::Qt::WA_MacShowFocusRect, 0);
	auto clearAction = searchLine->addAction(QIcon(":images/clear.svg"), QLineEdit::TrailingPosition);
	QObject::connect(clearAction, &QAction::triggered, [=] {
		searchLine->setText(QString());
		if (tabs->currentIndex() == fakeTabIndex) {
			QSettings settings;
			settings.beginGroup("Window:" + this->identifier());
			const auto index = settings.value("currentTabIndex").toInt();
			tabs->setCurrentIndex(index);
			emit tabs->tabBarClicked(index);
			settings.endGroup();
		}
		clearAction->setVisible(false);
	});
	clearAction->setVisible(false);
	QObject::connect(searchLine, &QLineEdit::textChanged, [=](const QString &text) {
		clearAction->setVisible(tabs->currentIndex() == fakeTabIndex || !text.isEmpty());
	});
	QObject::connect(searchLine, &QLineEdit::returnPressed, [=]() {
		const auto pattern = searchLine->text();
		// TODO: validate input
		if (!pattern.isEmpty()) {
			searchResults->setModel(new NetworkLibraryModel(this, pattern));
			tabs->setCurrentIndex(fakeTabIndex);
			diagrams->setVisible(false);
			knots->setVisible(false);
			searchResults->setVisible(true);
			searchLine->addPattern(pattern);
		}
	});
	top->addWidget(searchLine);
	top->addItem(new QSpacerItem(20, 0, QSizePolicy::Minimum, QSizePolicy::Minimum));
	vlayout->addWidget(diagrams);
	vlayout->addWidget(knots);
	vlayout->addWidget(searchResults);

	{
		QSettings settings;
		settings.beginGroup("Window:" + this->identifier());
		tabs->setCurrentIndex(settings.value("currentTabIndex").toInt());
		settings.endGroup();
	}
	QObject::connect(tabs, &QTabBar::tabBarClicked, [=](int index) {
		diagrams->setVisible(index == 0);
		knots->setVisible(index == 1);
		if (index == 0) {
			diagrams->setFocus();
		} else if (index == 1) {
			knots->setFocus();
		}
		searchResults->setVisible(false);
		QSettings settings;
		settings.beginGroup("Window:" + this->identifier());
		settings.setValue("currentTabIndex", index);
		settings.endGroup();
		settings.sync();
	});
	emit tabs->tabBarClicked(tabs->currentIndex());

	setWindowTitle("Knot Library");
	this->resize(780, 500);

	this->createFileMenu();
	this->restoreParameters();
}

QListView *LibraryWindow::createList(const QString &suffix) {
	QList<QListWidgetItem*> items;
	for (QDirIterator iter(":data", QDirIterator::Subdirectories); iter.hasNext(); ) {
		const auto fileName = iter.next();
		if (!fileName.endsWith(suffix)) {
			continue;
		}

		QRegularExpression re("^(\\d+)_(\\d+)\\....$");
		QRegularExpressionMatch match = re.match(iter.fileName());
		int index = std::numeric_limits<int>::max();
		if (match.hasMatch()) {
			index = 1000 * match.captured(1).toInt() + 1 * match.captured(2).toInt();
		}
		items.append(new FileDataItem(iter.filePath(), index));
	}
	std::sort(items.begin(), items.end(), [](const QListWidgetItem *it0, const QListWidgetItem *it1) {
		return *it0 < *it1;
	});
	return new LibraryListView(items);
}

}

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

#include <QtCore/QDirIterator>
#include <QtCore/QSettings>
#include <QtCore/QStringListModel>
#include <QtGui/QAction>
#include <QtGui/QDesktopServices>
#include <QtGui/QKeyEvent>
#include <QtWidgets/QCompleter>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMenu>
#include <QtWidgets/QVBoxLayout>

#include "LibraryModel.h"
#include "LibraryWindow.h"
#include "NetworkManager.h"

namespace KE::Qt {

namespace {

class LibraryListView : public QListView {

public:
	LibraryListView(const QList<QListWidgetItem *> &items = QList<QListWidgetItem *>()) {
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
			if (auto item = static_cast<const LibraryModel *>(this->model())->dataItem(index)) {
				item->open();
			}
		});
		QObject::connect(this->selectionModel(), &QItemSelectionModel::currentChanged, [this](const QModelIndex &current, const QModelIndex & /*previous*/) {
			if (current.isValid() && !this->selectionModel()->isSelected(current)) {
				this->selectionModel()->setCurrentIndex(QModelIndex(), QItemSelectionModel::ClearAndSelect);
			}
		});
	}

	~LibraryListView() {
		this->model()->deleteLater();
	}

	bool isEmpty() const {
		return this->model()->rowCount() == 0;
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
	void leaveEvent(QEvent *) override {
		this->selectionModel()->setCurrentIndex(QModelIndex(), QItemSelectionModel::ClearAndSelect);
	}

	void keyPressEvent(QKeyEvent *event) override {
		if (event->key() != ::Qt::Key_Enter && event->key() != ::Qt::Key_Return) {
			QListView::keyPressEvent(event);
			return;
		}

		if (auto item = static_cast<const LibraryModel *>(this->model())->dataItem(this->currentIndex())) {
			item->open();
		}
	}
};

class SearchBar : public QLineEdit {

private:
	QString _latestPattern;
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
			QDesktopServices::openUrl(QUrl("https://knots.mathesis.fun/help/search"));
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

	const QString &latestPattern() const {
		return this->_latestPattern;
	}

	void addPattern(const QString &pattern) {
		if (pattern.isEmpty() || pattern == this->_latestPattern) {
			return;
		}
		this->_latestPattern = pattern;

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

}// namespace

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
			if (pattern != searchLine->latestPattern() || searchResults->isEmpty()) {
				searchResults->setModel(new NetworkLibraryModel(this, pattern));
			}
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
	QList<QListWidgetItem *> items;
	for (QDirIterator iter(":data", QDirIterator::Subdirectories); iter.hasNext();) {
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

}// namespace KE::Qt

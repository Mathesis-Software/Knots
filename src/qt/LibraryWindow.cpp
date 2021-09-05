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
#include <QtGui/QKeyEvent>
#include <QtGui/QPainter>
#include <QtWidgets/QListWidget>

#include "KnotEditorApplication.h"
#include "LibraryWindow.h"
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

		int minX = std::numeric_limits<int>::max();
		int minY = std::numeric_limits<int>::max();
		int maxX = std::numeric_limits<int>::min();
		int maxY = std::numeric_limits<int>::min();
		for (const auto &vertex : this->diagram.vertices()) {
			minX = std::min(minX, vertex->x());
			minY = std::min(minY, vertex->y());
			maxX = std::max(maxX, vertex->x());
			maxY = std::max(maxY, vertex->y());
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
			const QPointF center(scale * vertex->x() + deltaX, scale * vertex->y() + deltaY);
			painter.drawEllipse(center, radius, radius);
		}

		painter.end();
	}

private:
	void drawEdge(QPainter &painter, const TwoD::Diagram::Edge &edge, float scale, float dX, float dY) {
		float deltaX = edge.end->x() - edge.start->x();
		float deltaY = edge.end->y() - edge.start->y();
		float hyp = hypotf(deltaX, deltaY);

		deltaX = 20 * deltaX / hyp;
		deltaY = 20 * deltaY / hyp;

		float x0 = edge.start->x(),
					y0 = edge.start->y(),
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

		x1 = edge.end->x();
		y1 = edge.end->y();

		if ((x1 - x0) * deltaX + (y1 - y0) * deltaY > 0) {
			painter.drawLine(QPointF(x0 * scale + dX, y0 * scale + dY), QPointF(x1 * scale + dX, y1 * scale + dY));
		}
	}
};

class DataItem : public QListWidgetItem {

public:
	const QString fullName;

private:
	int index;

public:
	DataItem(const QDirIterator &iter) : fullName(iter.filePath()), index(1000000) {
		QRegularExpression re("^(\\d+)_(\\d+)\\....$");
		QRegularExpressionMatch match = re.match(iter.fileName());
		if (match.hasMatch()) {
			this->index = 1000 * match.captured(1).toInt() + 1 * match.captured(2).toInt();
		}

		QResource resource(this->fullName);
		rapidjson::Document doc;
		doc.Parse(reinterpret_cast<const char*>(resource.data()), resource.size());
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
		}

		this->setTextAlignment(::Qt::AlignCenter);
		this->setSizeHint(QSize(120, 145));
		QIcon icon;
		icon.addPixmap(pixmap, QIcon::Normal);
		icon.addPixmap(pixmap, QIcon::Selected);
		this->setIcon(icon);
	}

	bool operator < (const QListWidgetItem &other) const override {
		const DataItem &data = dynamic_cast<const DataItem&>(other);
		return this->index < data.index || (this->index == data.index && this->fullName < data.fullName);
	}
};

class LibraryListWidget : public QListWidget {

private:
	void leaveEvent(QEvent*) override {
		const auto selected = this->selectedItems();
		for (const auto &item : selected) {
			item->setSelected(false);
		}
	}

	void keyPressEvent(QKeyEvent *event) override {
		if (event->key() != ::Qt::Key_Enter && event->key() != ::Qt::Key_Return) {
			QListWidget::keyPressEvent(event);
			return;
		}

		const auto selected = this->selectedItems();
		if (selected.size() != 1) {
			return;
		}
		const DataItem *data = dynamic_cast<const DataItem*>(selected[0]);
		KnotEditorApplication::openFile(data->fullName);
	}
};

}

LibraryWindow::LibraryWindow() {
	auto tabs = new QTabWidget(this);
	this->setCentralWidget(tabs);
	tabs->addTab(createList(".dgr"), "Diagrams");
	tabs->addTab(createList(".knt"), "Knots");

	{
		QSettings settings;
		settings.beginGroup("Window:" + this->identifier());
		const int index = settings.value("currentTabIndex").toInt();
		if (index > 0) {
			tabs->setCurrentIndex(index);
		}
		settings.endGroup();
	}
	QObject::connect(tabs, &QTabWidget::currentChanged, [this](int index) {
		QSettings settings;
		settings.beginGroup("Window:" + this->identifier());
		settings.setValue("currentTabIndex", index);
		settings.endGroup();
		settings.sync();
	});

	setWindowTitle("Knot Library");
	this->resize(780, 500);

	this->createFileMenu();
	this->restoreParameters();
}

QWidget *LibraryWindow::createList(const QString &suffix) {
	auto list = new LibraryListWidget();
	list->setMouseTracking(true);
	list->setViewMode(QListWidget::IconMode);
	list->setResizeMode(QListWidget::Adjust);
	list->setContentsMargins(0, 0, 0, 0);
	list->setIconSize(QSize(100, 100));
	list->setSpacing(5);
	list->setUniformItemSizes(true);
	list->setStyleSheet("QListWidget{background:#d8d8d8;} QListWidget::item{background:white;border:1px solid #c0c0c0;color:#808080;} QListWidget::item::selected{border:2px solid #404040;}");

	for (QDirIterator it(":data", QDirIterator::Subdirectories); it.hasNext(); ) {
		const auto fileName = it.next();
		if (!fileName.endsWith(suffix)) {
			continue;
		}
		list->addItem(new DataItem(it));
	}
	list->sortItems();
	QObject::connect(list, &QListWidget::itemEntered, [](QListWidgetItem *item) {
		item->setSelected(true);
	});
	QObject::connect(list, &QListWidget::itemClicked, [](QListWidgetItem *item) {
		const DataItem *data = dynamic_cast<const DataItem*>(item);
		KnotEditorApplication::openFile(data->fullName);
	});
	return list;
}

}

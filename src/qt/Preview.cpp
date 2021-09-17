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

#include "Preview.h"

namespace KE::Qt {

KnotPreview::KnotPreview(const ThreeD::Knot &knot) : knot(knot) {
}

void KnotPreview::paint(QPixmap &pixmap) {
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

DiagramPreview::DiagramPreview(const TwoD::Diagram &diagram) : diagram(diagram) {
}

void DiagramPreview::paint(QPixmap &pixmap) {
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

void DiagramPreview::drawEdge(QPainter &painter, const TwoD::Diagram::Edge &edge, float scale, float dX, float dY) {
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

}

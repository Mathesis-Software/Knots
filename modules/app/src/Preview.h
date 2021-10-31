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

#ifndef __KE_QT_PREVIEW_H__
#define __KE_QT_PREVIEW_H__

#include <QtGui/QPainter>

#include <Diagram.h>
#include <Knot.h>

namespace KE::Qt {

class KnotPreview {

private:
	const ThreeD::Knot &knot;

public:
	KnotPreview(const ThreeD::Knot &knot);
	void paint(QPixmap &pixmap);
};

class DiagramPreview {

private:
	const TwoD::Diagram &diagram;

public:
	DiagramPreview(const TwoD::Diagram &diagram);
	void paint(QPixmap &pixmap);

private:
	void drawEdge(QPainter &painter, const TwoD::Diagram::Edge &edge, float scale, float dX, float dY);
};

}// namespace KE::Qt

#endif /* __KE_QT_PREVIEW_H__ */

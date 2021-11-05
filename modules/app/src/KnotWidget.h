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

#ifndef __KNOTWIDGET_H__
#define __KNOTWIDGET_H__

#include <QtCore/QThread>

#include <KnotWrapper.h>

#include "GLWidget.h"

namespace KE::GL {

class KnotSurface;
class SeifertSurface;

}// namespace KE::GL

namespace KE::Qt {

class KnotWidget;

class SmoothingThread : public QThread {

	Q_OBJECT

private:
	ThreeD::KnotWrapper &knot;

public:
	SmoothingThread(KnotWidget *widget);

signals:
	void knotChanged();

private:
	void run() override;
};

class KnotWidget : public GLWidget {

	Q_OBJECT

public:
	ThreeD::KnotWrapper knot;

private:
	SmoothingThread smoothingThread;

public:
	KnotWidget(QWidget *parent, const rapidjson::Value &doc);
	KnotWidget(QWidget *parent, const TwoD::Diagram &diagram, std::size_t width, std::size_t height);

private:
	void init();
	void prepareMatrix(double *matrix, bool inverse) const override;
	void rotate(double dx, double dy, double dz) override;

public:
	const Color &backgroundColor() const override;

	void saveKnot(std::ostream &os);

	void startSmoothing();
	void stopSmoothing();
	void stopSmoothingAndWait();
	bool isSmoothingInProgress() const;

	void setSeifertSurfaceVisibility(bool visible);
	void moveSeifertBasePoint(double distance);

	void setLength();
	void setNumberOfPoints();

	void onKnotChanged(bool force);

signals:
	void setActionTip(const QString &tip);
	void actionsUpdated();
};

}

#endif /* __KNOTWIDGET_H__ */

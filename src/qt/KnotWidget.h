/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at

 *   http://www.apache.org/licenses/LICENSE-2.0

 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 *
 * Author: Nikolay Pultsin <geometer@geometer.name>
 */

#ifndef __KNOTWIDGET_H__
#define __KNOTWIDGET_H__

#include <QtCore/QThread>

#include "GLWidget.h"
#include "../math/knotWrapper/KnotWrapper.h"

namespace KE::GL {

class KnotSurface;
class SeifertSurface;

}

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

friend class KnotOptionsDialog;
friend class SmoothingThread;

Q_OBJECT

private:
	ThreeD::KnotWrapper _knot;
	SmoothingThread smoothingThread;

public:
	KnotWidget(QWidget *parent, const rapidjson::Document &doc);
	KnotWidget(QWidget *parent, const TwoD::Diagram &diagram, std::size_t width, std::size_t height);

private:
	void init();

public:
	const ThreeD::KnotWrapper &knot() const { return this->_knot; }
	const Color &backgroundColor() const override;

  void saveKnot(std::ostream &os);
	bool isKnotSaved() const;

  void startSmoothing();
  void stopSmoothing();
  void stopSmoothingAndWait();
	bool isSmoothingInProgress() const;

	bool isSeifertSurfaceVisible() const;
  void setSeifertSurfaceVisibility(bool visible);
  void moveSeifertBasePoint(double distance);

  void setCaption(const QString &caption) { this->_knot.setCaption(caption.toStdString()); }
  void setLength();
  void setNumberOfPoints();

	void onKnotChanged(bool force);

signals:
	void setActionTip(const QString &tip);
	void actionsUpdated();
};

}

#endif /* __KNOTWIDGET_H__ */

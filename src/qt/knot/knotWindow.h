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

#ifndef __KNOTWINDOW_H__
#define __KNOTWINDOW_H__

#include <QtCore/QThread>

#include "../gl/GLWindow.h"
#include "../../gl/surface/Surface.h"
#include "../../math/knotWrapper/KnotWrapper.h"
#include "../../math/seifert/seifert.h"

class KnotWidget;

namespace KE { namespace GL {

class KnotSurface;
class SeifertSurface;

}}

class paramWindow;
class diagramWindow;
class knotWindow;

class SmoothingThread : public QThread {

Q_OBJECT

private:
	knotWindow &knot;

public:
	SmoothingThread(knotWindow &knot);

signals:
	void knotChanged();

private:
	void run() override;
};

class knotWindow : public GLWindow {

friend class SmoothingThread;

Q_OBJECT

private:
	KE::ThreeD::KnotWrapper knot;

	std::shared_ptr<KE::GL::KnotSurface> knotSurface;
	KE::ThreeD::Point seifertStartPoint;
	std::shared_ptr<KE::GL::SeifertSurface> seifertSurface;
	SmoothingThread smoothingThread;

  QMenu *mathMenu;
  QMenu *viewMenu;

  friend class paramWindow;
  paramWindow *mth;

  void init();
  void initMenu();

  bool continuousSmoothing;
  int smoothSteps;
  int redrawAfter;
  void startSmooth(int, int, bool = true);

  void doSmooth();
	void onKnotChanged();

	bool isSaved() const override;

  void saveIt(std::ostream&);

private:
	void runColorDialog(const QString &title, std::function<QColor()> getter, std::function<void(const QColor&)> setter);

  void toggleSeifertSurfaceVisibility();
	KnotWidget *knotWidget() const;

private slots:
  void stop();
  void math();
  void smooth();
  void setLength();
  void setNumberOfPoints();
  void decreaseEnergy();
  void setThickness();
  void setBgColor();
  void setKnotColor();
  void setSeifertFrontColor();
  void setSeifertBackColor();

public:
  knotWindow(const rapidjson::Document &doc);
  knotWindow(const diagramWindow &diagram);
  ~knotWindow();

	void closeEvent(QCloseEvent *event);

private:
	QString fileFilter() const override { return "Knot files (*.knt)"; }
	void updateActions() override;
	void rename() override;

  void moveSeifertBasePoint(double distance);
};

#endif /* __KNOTWINDOW_H__ */

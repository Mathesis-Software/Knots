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

#include "../gl/GLWidget.h"
#include "../../math/knotWrapper/KnotWrapper.h"

namespace KE { namespace GL {

class KnotSurface;
class SeifertSurface;

}}

class KnotWidget : public GLWidget {

Q_OBJECT

private:
	KE::ThreeD::KnotWrapper &knot;

	std::shared_ptr<KE::GL::KnotSurface> knotSurface;
	KE::ThreeD::Point seifertStartPoint;
	std::shared_ptr<KE::GL::SeifertSurface> seifertSurface;

public:
	KnotWidget(QWidget *parent, KE::ThreeD::KnotWrapper &knot);
	const KE::GL::Color &backgroundColor() const override;

	bool isSeifertSurfaceVisible() const;
  void toggleSeifertSurfaceVisibility();
  void moveSeifertBasePoint(double distance);

  void setLength();
  void setNumberOfPoints();
  void setThickness();
  void setBgColor();
  void setKnotColor();
  void setSeifertFrontColor();
  void setSeifertBackColor();

	void onKnotChanged();

signals:
	void actionsUpdated();
};

#endif /* __KNOTWIDGET_H__ */

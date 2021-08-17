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

#include "../gl/GLWindow.h"
#include "../../math/knotWrapper/KnotWrapper.h"

class KnotWidget;

class paramWindow;
class diagramWindow;
class knotWindow;

class knotWindow : public GLWindow {

private:
	KE::ThreeD::KnotWrapper knot;

  QMenu *mathMenu;
  QMenu *viewMenu;

  friend class paramWindow;
  paramWindow *mth;

  void init();
  void initMenu();

	bool isSaved() const override;

  void saveIt(std::ostream&);

private:
	KnotWidget *knotWidget() const;

  void math();

public:
  knotWindow(const rapidjson::Document &doc);
  knotWindow(const diagramWindow &diagram);
  ~knotWindow();

private:
	void closeEvent(QCloseEvent *event) override;

	QString fileFilter() const override { return "Knot files (*.knt)"; }
	void updateActions() override;
	void rename() override;
};

#endif /* __KNOTWINDOW_H__ */

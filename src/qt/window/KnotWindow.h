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

#include "Window.h"
#include "../../math/knotWrapper/KnotWrapper.h"

namespace KE { namespace Qt {

class KnotWidget;
class KnotMathDialog;
class DiagramWidget;

class KnotWindow : public Window {

private:
  QMenu *mathMenu;
  QMenu *viewMenu;

  friend class KnotMathDialog;
  KnotMathDialog *mth;

  void init(KnotWidget *widget);
  void initMenu();

	QImage exportImage() const override;

	bool isSaved() const override;
  void saveIt(std::ostream&) override;

private:
	KnotWidget *knotWidget() const;

  void math();

public:
  KnotWindow(const rapidjson::Document &doc);
  KnotWindow(const DiagramWidget &diagramWidget);
  ~KnotWindow();

private:
	void closeEvent(QCloseEvent *event) override;

	QString fileFilter() const override { return "Knot files (*.knt)"; }
	void updateActions() override;
	void rename() override;
};

}}

#endif /* __KNOTWINDOW_H__ */

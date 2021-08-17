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

#ifndef __DIAGRAMWINDOW_H__
#define __DIAGRAMWINDOW_H__

#include <QtWidgets/QMenu>

#include "abstractWindow.h"
#include "../widget/DiagramWidget.h"

class diagramWindow : public abstractWindow {

private:
	QMenu *actionsMenu;

	void init(DiagramWidget *widget);

	QString fileFilter() const override { return "Diagram files (*.dgr)"; }

	void printIt(QPrinter*) override;

	void setMode(DiagramWidget::EditorMode mode);

	void convert();
	void clear();
	void simplify();

	void rename() override;

public:
	diagramWindow(const rapidjson::Document &doc);
	diagramWindow();
	~diagramWindow();

	DiagramWidget *diagramWidget() const { return (DiagramWidget*)this->centralWidget(); }

	void saveIt(std::ostream&) override;
	bool isSaved() const override;

	void updateActions() override;
};

#endif /* __DIAGRAMWINDOW_H__ */

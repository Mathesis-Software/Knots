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

#ifndef __KE_QT_DIAGRAM_WINDOW_H__
#define __KE_QT_DIAGRAM_WINDOW_H__

#include <QtWidgets/QDialog>

#include "DiagramWidget.h"
#include "Window.h"

namespace KE::Qt {

class DiagramWindow : public Window {

Q_OBJECT

private:
	void init(DiagramWidget *widget);

	QString fileFilter() const override { return "Diagram files (*.dgr)"; }

	QImage exportImage() const override;

	void setMode(DiagramWidget::EditorMode mode);

	void convert();
	void clear();
	void simplify();

	void rename() override;

	void showPropertiesDialog();

public:
	DiagramWindow(const rapidjson::Document &doc);
	DiagramWindow();

	DiagramWidget *diagramWidget() const { return (DiagramWidget*)this->centralWidget(); }

	void saveIt(std::ostream&) override;
	bool isSaved() const override;

signals:
	void raisePropertiesDialog();
};

class DiagramPropertiesDialog : public QDialog {

public:
	DiagramPropertiesDialog(DiagramWindow &window);
};

}

#endif /* __KE_QT_DIAGRAM_WINDOW_H__ */

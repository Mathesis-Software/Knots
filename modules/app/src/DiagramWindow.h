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
	DiagramWindow(const rapidjson::Value &doc, const QString &filename);
	DiagramWindow();

	DiagramWidget *diagramWidget() const { return (DiagramWidget *) this->centralWidget(); }

	void saveIt(std::ostream &) override;
	bool isSaved() const override;

signals:
	void raisePropertiesDialog();
};

class DiagramPropertiesDialog : public QDialog {

public:
	DiagramPropertiesDialog(DiagramWindow &window);
};

}// namespace KE::Qt

#endif /* __KE_QT_DIAGRAM_WINDOW_H__ */

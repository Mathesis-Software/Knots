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

#ifndef __KE_QT_KNOT_WINDOW_H__
#define __KE_QT_KNOT_WINDOW_H__

#include <QtWidgets/QDialog>

#include "Window.h"
#include "../ke/KnotWrapper.h"

namespace KE::Qt {

class KnotWidget;
class DiagramWidget;

class KnotWindow : public Window {

Q_OBJECT

private:
	QMenu *knotMenu;

public:
	KnotWindow(const rapidjson::Document &doc);
	KnotWindow(const DiagramWidget &diagramWidget);
	~KnotWindow();

	KnotWidget *knotWidget() const;

private:
	void init(KnotWidget *widget);
	void initMenu();

	QImage exportImage() const override;

	bool isSaved() const override;
	void saveIt(std::ostream&) override;

	void closeEvent(QCloseEvent *event) override;

	QString fileFilter() const override { return "Knot files (*.knt)"; }
	void rename() override;

	void showMathDialog();
	void showOptionsDialog();

signals:
	void raiseMathDialog();
	void raiseOptionsDialog();
};

class KnotMathDialog : public QDialog {

public:
	KnotMathDialog(KnotWindow &window);
};

class KnotOptionsDialog : public QDialog {

public:
	KnotOptionsDialog(KnotWindow &window);
};

}

#endif /* __KE_QT_KNOT_WINDOW_H__ */

/*
 * Copyright (c) 1995-2021, Nikolay Pultsin <geometer@geometer.name>
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

#ifndef __KE_QT_KNOT_EDITOR_APPLICATION_H__
#define __KE_QT_KNOT_EDITOR_APPLICATION_H__

#include <QtWidgets/QApplication>

namespace KE::Qt {

class KnotEditorApplication : public QApplication {

private:
	bool windowsListSaved;

public:
	KnotEditorApplication(int &argc, char **argv);
	bool event(QEvent *event) override;

	QWidget *library();
	QWidget *newDiagram();
	void diagramFromCode();
	QWidget *openFile();
	QWidget *openFile(const QString &filename);
	void exitApplication();

private:
	void closeStartWindow();
};

}

#endif /* __KE_QT_KNOT_EDITOR_APPLICATION_H__ */

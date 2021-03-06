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

#ifndef __KE_QT_WINDOW_H__
#define __KE_QT_WINDOW_H__

#include "BaseWindow.h"

namespace KE::Qt {

class Window : public BaseWindow {

Q_OBJECT

private:
	QToolBar *toolbar;
	QString _filename;

protected:
	Window(const QString &filename = QString());

public:
	QString identifier() const override;
	void save();
	virtual void rename() = 0;
	void exportPNG();

protected:
	void closeEvent(QCloseEvent *event) override;

	virtual void saveIt(std::ostream &os) = 0;
	virtual bool isSaved() const = 0;
	bool saveBeforeClosing();

	virtual QImage exportImage() const = 0;

	QAction *addToolbarAction(const QString &iconFilename, const QString &text, const std::function<void()> &functor);
	void addToolbarSeparator();
	QAction *registerAction(QAction *action, std::function<void(QAction&)> controller);

	virtual QString fileFilter() const = 0;

signals:
	void closing();
	void contentChanged();
};

}

#endif /* __KE_QT_WINDOW_H__ */

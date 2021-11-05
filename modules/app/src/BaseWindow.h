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

#ifndef __KE_QT_BASE_WINDOW_H__
#define __KE_QT_BASE_WINDOW_H__

#include <QtWidgets/QMainWindow>

namespace KE::Qt {

class BaseWindow : public QMainWindow {

protected:
	BaseWindow();
	void restoreParameters();
	void createFileMenu();
	void closeEvent(QCloseEvent *event) override;

public:
	virtual QString identifier() const {
		return QString();
	}
};

}

#endif /* __KE_QT_BASE_WINDOW_H__ */

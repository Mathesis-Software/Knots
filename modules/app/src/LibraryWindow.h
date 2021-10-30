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

#ifndef __KE_QT_LIBRARY_WINDOW_H_
#define __KE_QT_LIBRARY_WINDOW_H_

#include <QtWidgets/QListView>

#include "BaseWindow.h"

namespace KE::Qt {

class NetworkManager;

class LibraryWindow : public BaseWindow {

private:
	NetworkManager *_networkManager;

public:
	LibraryWindow();
	QString identifier() const override {
		return "::LIBRARY::";
	}
	NetworkManager *networkManager() { return this->_networkManager; }

private:
	QListView *createList(const QString &suffix);
};

}

#endif /* __KE_QT_LIBRARY_WINDOW_H_ */
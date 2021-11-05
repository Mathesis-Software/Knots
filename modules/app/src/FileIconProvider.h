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

#ifndef __KE_QT_FILE_ICON_PROVIDER_H__
#define __KE_QT_FILE_ICON_PROVIDER_H__

#include <QtWidgets/QFileIconProvider>

namespace KE::Qt {

class FileIconProvider : public QFileIconProvider {

public:
	static FileIconProvider *instance();

private:
	static FileIconProvider *_instance;

private:
	FileIconProvider();

public:
	QIcon icon(const QFileInfo &) const override;

private:
	const QIcon diagramIcon;
	const QIcon knotIcon;
};

}

#endif /* __KE_QT_FILE_ICON_PROVIDER_H__ */

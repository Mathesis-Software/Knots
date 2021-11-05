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
 *
 * The idea taken from https://stackoverflow.com/questions/5006547/qt-best-practice-for-a-single-instance-app-protection
 */

#ifndef __QT_IPC_CONTROLLER_H__
#define __QT_IPC_CONTROLLER_H__

#include <QtCore/QSharedMemory>
#include <QtCore/QSystemSemaphore>
#include <QtCore/QThread>

namespace IPC {

class Controller;

class MessageReceiver : public QThread {

public:
	MessageReceiver(Controller *parent);
	void run() override;
};

class Controller : public QObject {

	friend class MessageReceiver;

	Q_OBJECT

public:
	enum Role {
		unknown,
		primary,
		secondary
	};

private:
	QSharedMemory sharedMemory;
	QSystemSemaphore memorySemaphore;
	QSystemSemaphore messageSemaphore;
	volatile Role _role;
	MessageReceiver *receiver;

public:
	Controller(const QString &key, QObject *parent);
	~Controller();

	Role role() const { return this->_role; }

	bool sendMessage(const QStringList &message);

signals:
	void messageReceived(const QStringList &message);

private:
	Controller(const Controller &) = delete;
	void operator=(const Controller &) = delete;
};

}

#endif /* __QT_IPC_CONTROLLER_H__ */

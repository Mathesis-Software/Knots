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
 *
 * The idea taken from https://stackoverflow.com/questions/5006547/qt-best-practice-for-a-single-instance-app-protection
 */

#include <cstring>

#include <QtCore/QCryptographicHash>

#include "IPCController.h"

namespace IPC {

namespace {

QString generateKey(const QString& key, const QString& salt) {
	QByteArray data;
	data.append(key.toUtf8());
	data.append(salt.toUtf8());
	return QCryptographicHash::hash(data, QCryptographicHash::Sha1).toHex();
}

}

MessageReceiver::MessageReceiver(Controller *controller) : QThread(controller) {
}

void MessageReceiver::run() {
	Controller *controller = static_cast<Controller*>(this->parent());
	while (true) {
		controller->messageSemaphore.acquire();
		if (this->isInterruptionRequested()) {
			break;
		}

		QString message;
		controller->memorySemaphore.acquire();
		if (controller->sharedMemory.lock()) {
			message = QString(reinterpret_cast<char*>(controller->sharedMemory.data()));
			std::memset(controller->sharedMemory.data(), 0, controller->sharedMemory.size());
			controller->sharedMemory.unlock();
		}
		controller->memorySemaphore.release();
		if (!message.isEmpty()) {
			emit controller->messageReceived(message);
		}
	}
}

Controller::Controller(const QString& key, QObject *parent) :
	QObject(parent),
 	sharedMemory(generateKey(key, "sharedMemory")),
	memorySemaphore(generateKey(key, "memorySemaphore"), 1),
	messageSemaphore(generateKey(key, "messageSemaphore"), 0, QSystemSemaphore::Create),
	_role(Role::unknown),
	receiver(nullptr)
{
	this->memorySemaphore.acquire();

	// fix Linux issue: these two lines drop sharedMemory lock acquired by crashed app
	// see http://habrahabr.ru/post/173281/ for details
	this->sharedMemory.attach();
	this->sharedMemory.detach();

	this->_role = this->sharedMemory.attach() ? Role::secondary : Role::primary;
	this->sharedMemory.detach();
	if (this->_role == Role::primary) {
		if (this->sharedMemory.create(1024)) {
			this->sharedMemory.lock();
			std::memset(this->sharedMemory.data(), 0, this->sharedMemory.size());
			this->sharedMemory.unlock();
		} else {
			this->_role = Role::unknown;
		}
	}
	if (this->_role == Role::primary) {
		this->receiver = new MessageReceiver(this);
		this->receiver->start();
	}
	this->memorySemaphore.release();
}

Controller::~Controller() {
	this->memorySemaphore.acquire();
	if (this->receiver) {
		this->receiver->requestInterruption();
		this->messageSemaphore.release(1);
		this->receiver->wait();
	}
	this->sharedMemory.detach();
	this->memorySemaphore.release();
}

bool Controller::sendMessage(const QString &message) {
	if (message.isEmpty()) {
		return false;
	}
	const std::string data = message.toStdString();

	bool sent = false;
	this->memorySemaphore.acquire();
	if (this->sharedMemory.attach()) {
		if (data.size() < static_cast<std::size_t>(this->sharedMemory.size())) {
			this->sharedMemory.lock();
			std::strcpy(reinterpret_cast<char*>(this->sharedMemory.data()), message.toStdString().c_str());
			this->sharedMemory.unlock();
			this->sharedMemory.detach();
			this->messageSemaphore.release(1);
			sent = true;
		} else {
			this->sharedMemory.detach();
		}
	}
	this->memorySemaphore.release();
	return sent;
}

}

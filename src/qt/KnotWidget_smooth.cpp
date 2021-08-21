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

#include "KnotWidget.h"

namespace KE::Qt {

void KnotWidget::startSmoothing() {
  if (!this->smoothingThread.isRunning()) {
		this->smoothingThread.start();
		emit setActionTip("Smoothing…");
		emit actionsUpdated();
	}
}

void KnotWidget::stopSmoothing() {
  if (this->smoothingThread.isRunning()) {
		this->smoothingThread.requestInterruption();
		emit setActionTip(QString());
		emit actionsUpdated();
  }
}

void KnotWidget::stopSmoothingAndWait() {
	if (this->smoothingThread.isRunning()) {
		this->smoothingThread.requestInterruption();
		this->smoothingThread.wait();
	}
}

SmoothingThread::SmoothingThread(KnotWidget *widget) : knot(widget->_knot) {
	connect(this, &SmoothingThread::knotChanged, [widget] { widget->onKnotChanged(false); });
	connect(this, &SmoothingThread::finished, [widget] { widget->onKnotChanged(true); });
}

void SmoothingThread::run() {
	this->setPriority(LowPriority);
	while (true) {
		if (this->isInterruptionRequested()) {
			this->quit();
			break;
		}
		this->msleep(20);
		for (int i = 0; i < 20; ++i) {
			this->knot.decreaseEnergy();
		}
		emit knotChanged();
	}
}

}

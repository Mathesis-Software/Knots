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

#ifndef __KNOTWINDOW_MATH_H__
#define __KNOTWINDOW_MATH_H__

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QDialog>

#include "knotWindow.h"
#include "../../math/computables/computable.h"

class paramWindow : public QDialog {

	Q_OBJECT

private:
 
	knotWindow *Parent;
	int nLabels;
	class parameterLabel **pLabels;
	void closeEvent(QCloseEvent*);

public:

	paramWindow(knotWindow*);
	~paramWindow();
	void recompute();
};

class parameterLabel : public QWidget {

Q_OBJECT

private:

	QLabel *lbl;
	QCheckBox *chkbox;
	std::shared_ptr<KE::ThreeD::Computables::Computable> computable;

private slots:

	void doit();

public:

	parameterLabel(QDialog*, std::shared_ptr<KE::ThreeD::Computables::Computable>, int, int);
	~parameterLabel();

	void renew();
};

#endif /* __KNOTWINDOW_MATH_H__ */

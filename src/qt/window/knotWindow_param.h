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

#ifndef __KNOTWINDOW_PARAM_H__
#define __KNOTWINDOW_PARAM_H__

#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

class setupSmooth : public QDialog {

Q_OBJECT
  
private:

  setupSmooth (int*, int*);
  ~setupSmooth (void);

  QLineEdit *steps_label, *redraw_label;
  QPushButton *startButton, *cancelButton;
  
  int *steps, *redraw;

private slots:

  void test ();

public:

  static bool set (int*, int*);
};

#endif /* __KNOTWINDOW_PARAM_H__ */

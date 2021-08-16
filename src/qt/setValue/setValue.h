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

#ifndef __SETVALUE_H__
#define __SETVALUE_H__

#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>

int setInt (const char*, int, int, int);
double setDouble (const char*, double, double, double);

class setupInt : public QDialog {

Q_OBJECT
  
private:

  setupInt (int*, int, int);
  ~setupInt (void);

  QLineEdit *label;
  QPushButton *okButton, *cancelButton;
  
  int *current;
  int minValue, maxValue;

private slots:

  void test ();

public:

  friend int setInt (const char*, int, int, int);
};

class setupDouble : public QDialog {

Q_OBJECT
  
private:

  setupDouble (double*, double, double);
  ~setupDouble (void);

  QLineEdit *label;
  QPushButton *okButton, *cancelButton;
  
  double *current;
  double minValue, maxValue;

private slots:

  void test ();

public:

  friend double setDouble (const char*, double, double, double);
};

#endif /* __SETVALUE_H__ */

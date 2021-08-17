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

#include <cstdio>

#include "knotWindow.h"
#include "KnotWidget.h"
#include "knotWindow_math.h"
#include "knotWindow_param.h"
#include "../setValue/setValue.h"

void knotWindow::decreaseEnergy() {
  int steps, redraw_after;
  if (!setupSmooth::set (&steps, &redraw_after))
    return;
  
  startSmooth (steps, redraw_after, false);
}

setupSmooth::setupSmooth (int* st, int* rd) : QDialog (0) {
  this->setModal(true);
  setFixedSize (245, 150);

  QLabel *steps_text = new QLabel (this);
  steps_text->setGeometry (20, 20, 170, 30);
  steps_text->setText ("Number of iteration");
  steps_label = new QLineEdit (this);
  steps_label->setGeometry (180, 20, 50, 30);
  steps_label->setMaxLength (5);
  steps_label->setText ("100");

  QLabel *redraw_text = new QLabel (this);
  redraw_text->setGeometry (20, 55, 170, 30);
  redraw_text->setText ("Repaint after");
  redraw_label = new QLineEdit (this);
  redraw_label->setGeometry (180, 55, 50, 30);
  redraw_label->setMaxLength (5);
  redraw_label->setText ("4");

  startButton = new QPushButton ("Start", this);
  startButton->setGeometry (45, 105, 70, 30);
  connect (startButton, SIGNAL (clicked()), SLOT (test()));
  cancelButton = new QPushButton ("Cancel", this);
  cancelButton->setGeometry (135, 105, 70, 30);
  connect (cancelButton, SIGNAL (clicked()), SLOT (reject()));

  steps = st;
  redraw = rd;
}

setupSmooth::~setupSmooth() {
  delete steps_label;
  delete redraw_label;
  delete startButton;
  delete cancelButton;
}

void setupSmooth::test() {
  char trash [6];

  if (sscanf (steps_label->text().toStdString().c_str(), "%d%s", steps, trash) != 1)
    return;
  if (sscanf (redraw_label->text().toStdString().c_str(), "%d%s", redraw, trash) != 1)
    return;

  if ((*steps >= *redraw) && (*redraw > 0)) {
    accept();
    return;
  }

  if ((*steps < *redraw) && (*steps > 0)) {
    redraw_label->setText (steps_label->text());
    return;
  }

  if (*steps <= 0) {
    steps_label->setText ("1");
    redraw_label->setText ("1");
    return;
  }

  redraw_label->setText ("1");
}

bool setupSmooth::set (int *st, int *rd) {
  setupSmooth *setup = new setupSmooth (st, rd);
  return setup->exec();
}

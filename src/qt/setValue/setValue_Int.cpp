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

#include <stdio.h>

#include "setValue.h"

setupInt::setupInt (int* tmp, int min, int max) : QDialog (0) {
  this->setModal(true);
  setFixedSize (200, 115);

  label = new QLineEdit (this);
  label -> setGeometry (50, 20, 100, 30);
  label -> setMaxLength (10);
  QString s;
  s.setNum (*tmp);
  label -> setText (s);

  okButton = new QPushButton ("OK", this);
  okButton -> setGeometry (20, 70, 70, 30);
  connect (okButton, SIGNAL (clicked()), SLOT (test()));
  connect (label, SIGNAL (returnPressed()), SLOT (test()));
  cancelButton = new QPushButton ("Cancel", this);
  cancelButton -> setGeometry (110, 70, 70, 30);
  connect (cancelButton, SIGNAL (clicked()), SLOT (reject()));

  current = tmp;
  minValue = min;
  maxValue = max;
}

setupInt::~setupInt (void)
{
  delete label;
  delete okButton;
  delete cancelButton;
}

void setupInt::test ()
{
  char trash [11];
  int *result = new int;
  if (sscanf (label -> text ().toStdString().c_str(), "%d%s", result, trash) == 1)
  {
    if ( (*result >= minValue) && (*result <= maxValue) )
    {
      *current = *result;
      accept ();
    }
    else
    {
      if (*result > maxValue)
      {
        QString s;
        s.setNum (maxValue);
        label -> setText (s);
      }
      else
      {
        QString s;
        s.setNum (minValue);
        label -> setText (s);
      }
    }
  }
  delete result;
}

int setInt (const char *caption, int old, int min, int max)
{
  int *tmp = new int (old);
  setupInt *setup = new setupInt (tmp, min, max);
  setup -> setWindowTitle (caption);
  if (setup -> exec ())
    old = *tmp;
  delete setup;
  delete tmp;
  return old;
}

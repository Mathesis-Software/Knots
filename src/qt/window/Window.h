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

#ifndef __KE_QT_WINDOW_H__
#define __KE_QT_WINDOW_H__

#include <fstream>
#include <list>

#include <QtPrintSupport/QPrinter>
#include <QtWidgets/QMainWindow>

namespace KE { namespace Qt {

class Window : public QMainWindow {

private:
  static std::list<Window*> AWRegister;

public:
  static QWidget *newDiagram();
  static QWidget *openFile();
	static void exitApplication();

private:
  static bool closeAllWindows();

private:
  QToolBar *toolbar;
	std::map<QAction*,std::function<void(QAction&)>> actionsMap;

protected:
  void closeEvent(QCloseEvent*);

  virtual void printIt(QPrinter*) = 0;
  void print();

  void save();
  virtual void saveIt(std::ostream&) = 0;
  virtual bool isSaved() const = 0;
  int askForSave(void);

  QAction *addToolbarAction(const QString &iconFilename, const QString &text, const std::function<void()> &functor);
  void addToolbarSeparator();
  void complete();

	QAction *registerAction(QAction *action, std::function<void(QAction&)> controller);

public:
  Window();
  virtual ~Window();
	virtual void updateActions();
  virtual QString fileFilter() const = 0;

protected:
  virtual void rename() = 0;
};

}}

#endif /* __KE_QT_WINDOW_H__ */
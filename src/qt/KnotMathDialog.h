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

#ifndef __KE_QT_KNOT_MATH_DIALOG_H__
#define __KE_QT_KNOT_MATH_DIALOG_H__

#include <QtWidgets/QDialog>

namespace KE { namespace Qt {

class KnotWindow;

class KnotMathDialog : public QDialog {

private:
	KnotWindow &window;
	std::vector<std::function<void()>> callbacks;

public:
	KnotMathDialog(KnotWindow &window);

private:
	void recompute();
};

}}

#endif /* __KE_QT_KNOT_MATH_DIALOG_H__ */

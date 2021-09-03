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

#ifndef __KE_MATH_DIAGRAM_PROPERTY_H__
#define __KE_MATH_DIAGRAM_PROPERTY_H__

#include <list>

namespace KE::TwoD {

class Diagram;

}

namespace KE::TwoD::Math {

class Polynomial;

template<typename T>
class DiagramProperty {

public:
	virtual bool isApplicable(const Diagram &diagram) const = 0;
	virtual T value(const Diagram &diagram) const = 0;
};

class DTCode : public DiagramProperty<std::list<int>> {

public:
	bool isApplicable(const Diagram &diagram) const override;
	std::list<int> value(const Diagram &diagram) const override;
};

class AlexanderPolynomial : public DiagramProperty<Polynomial> {

public:
	bool isApplicable(const Diagram &diagram) const override;
	Polynomial value(const Diagram &diagram) const override;
};

}

#endif /* __KE_MATH_DIAGRAM_PROPERTY_H__ */

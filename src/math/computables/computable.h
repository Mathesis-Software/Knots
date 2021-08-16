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

#ifndef __COMPUTABLE_H__
#define __COMPUTABLE_H__

#include "../knot/Knot.h"

namespace KE { namespace ThreeD {

class KnotWrapper;

namespace Computables {

class Computable {

public:
	const std::string name;

private:
	const KnotWrapper &knot;
	std::shared_ptr<Knot::Snapshot> snapshot;
	double _value;

protected:
	virtual double compute(const Knot::Snapshot &snapshot) = 0;

public:
	Computable(const KnotWrapper &knot, const std::string &name) : name(name), knot(knot) {}
	virtual ~Computable() {}

	double value();
};

}}}

#endif /* __COMPUTABLE_H__ */

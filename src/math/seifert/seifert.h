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

#ifndef __SEIFERT_H__
#define __SEIFERT_H__

#include "../knot/Knot.h"

class seifert;
class seifert_list;
class seifert_ord;

namespace KE { namespace GL {

class SeifertSurface;

}}

class seifert_list {

private:
	seifert *basepoint, *value;
	seifert_list *prev, *next;
	bool label;

	void insert(seifert*);
	void insert_after(seifert*);

	seifert_list(seifert*, seifert*);
	~seifert_list();

	friend class seifert;
	friend class KE::GL::SeifertSurface;
};

class seifert_ord {

private:
	seifert *value;
	seifert_ord *prev, *next;

	seifert_ord *insert(seifert*);

	seifert_ord(seifert*);
	~seifert_ord();

	friend class seifert;
	friend class KE::GL::SeifertSurface;
};

class seifert {

private:
	const KE::ThreeD::Knot::Snapshot snapshot;
	const KE::ThreeD::Point point;
	const KE::ThreeD::Vector gradient;

	double localEps;
	seifert_list *neighborhood;
	seifert_ord *sord;

	void searchForNeighbor();
	void checkNeighborhood();
	void addPoint(const KE::ThreeD::Vector &direction);
	void addPoint60(const KE::ThreeD::Vector &direction);
	seifert_list *hasNeighbor(seifert*);
	void markUsed(seifert*, seifert*);
	void correction_local();
	void correction();

public:
	seifert(const KE::ThreeD::Knot::Snapshot &snapshot, const KE::ThreeD::Point &point, seifert* = nullptr);
	~seifert();

	friend class seifert_ord;
	friend class seifert_list;
	friend class KE::GL::SeifertSurface;
};

#endif /* __SEIFERT_H__ */

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

#ifndef __KNOT_H__
#define __KNOT_H__

#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include <rapidjson/document.h>

#include "primitives.h"

namespace KE {

namespace TwoD {

class Diagram;

}

namespace ThreeD {

class Knot {

public:
	class Snapshot {

	friend class Knot;

	private:
		const Knot &knot;
		const std::shared_ptr<std::vector<Point>> points;
		const std::shared_ptr<std::vector<double>> _edgeLengths;
		const std::size_t generation;

	private:
		Snapshot(const Knot &knot, const std::vector<Point> &points);

	public:
		bool isObsolete() const { return this->generation < this->knot.generation; }

		const Point &operator[](std::size_t	index) const { return (*this->points)[index]; }
		std::size_t size() const { return this->points->size(); }

		std::size_t next(std::size_t index) const {
			return index == this->points->size() - 1 ? 0 : index + 1;
		}
		std::size_t prev(std::size_t index) const {
			return index ? index - 1 : this->points->size() - 1;
		}

		const std::vector<double> &edgeLengths() const;
		double knotLength() const;
	};

private:
	class counting_lock {

	private:
		Knot &knot;

	public:
		counting_lock(Knot &knot) : knot(knot) {
			this->knot.dataChangeMutex.lock();
			this->knot.lockCount += 1;
		}
		~counting_lock() {
			this->knot.lockCount -= 1;
			this->knot.generation += 1;
			this->knot.dataChangeMutex.unlock();
		}
	};

public:
	std::string caption;

private:
	mutable std::recursive_mutex dataChangeMutex;
	std::mutex writeMethodMutex;
	std::vector<Point> _points;
	volatile std::size_t generation;
	mutable volatile std::size_t lockCount;
	mutable std::shared_ptr<Snapshot> latest;

public:
	Knot(const rapidjson::Document &doc);
	Knot(const TwoD::Diagram&, std::size_t width, std::size_t height);

	Snapshot snapshot() const;

	void decreaseEnergy();
	void setLength(double);
	void center();
	void normalize(std::size_t numberOfPoints);

	rapidjson::Document serialize() const;
	rapidjson::Document serialize(const double matrix[3][3]) const;

private:
	static std::vector<Point> normalizedPoints(const Snapshot &snapshot, std::size_t numberOfPoints);

private:
	Knot(const Knot&) = delete;
	Knot& operator = (const Knot&) = delete;
};

}}

#endif /* __KNOT_H__ */

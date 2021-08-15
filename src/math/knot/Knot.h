#ifndef __KNOT_H__
#define __KNOT_H__

#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include <rapidjson/document.h>

#include "primitives.h"
#include "../seifert/seifert.h"

namespace KE {

namespace GL {

class KnotSurface;

}

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
		bool isObsolete() const { return this->generation < this->knot.generation(); }

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
			this->knot._generation += 1;
			this->knot.dataChangeMutex.unlock();
		}
	};

public:
	std::string caption;

private:
	mutable std::recursive_mutex dataChangeMutex;
	std::mutex writeMethodMutex;
	std::vector<Point> _points;
	volatile std::size_t _generation;
	mutable volatile std::size_t lockCount;
	mutable std::shared_ptr<Snapshot> latest;

public:
	Knot(const rapidjson::Document &doc);
	Knot(const TwoD::Diagram&, std::size_t width, std::size_t height);

	Snapshot points() const;
	std::size_t generation() const { return this->_generation; }

	void decreaseEnergy();
	void setLength(double);
	void center();
	std::size_t numberOfPoints() const { return this->points().size(); }
	void normalize(std::size_t numberOfPoints);

	Vector seifertGradient(const Point &point) const;
	double minDist(const Point &point) const;

	rapidjson::Document save() const;
	rapidjson::Document save(const double matrix[3][3]) const;

private:
	std::vector<Point> normalizedPoints(const Snapshot &snapshot, std::size_t numberOfPoints) const;

private:
	Knot(const Knot&) = delete;
	Knot& operator = (const Knot&) = delete;
};

}}

#endif /* __KNOT_H__ */

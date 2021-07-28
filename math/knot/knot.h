#ifndef __KNOT_H__
#define __KNOT_H__

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "point.h"
#include "../seifert/seifert.h"

class diagram;

/***********************************************************************/

#define	addParameterClass(PARAM)	\
class PARAM : public Computable {	\
private:				\
	double compute() override;			\
public:					\
	PARAM (const Knot &knot, const std::string &name) : Computable(knot, name) {} \
}

#define	addParameterClassWithOrder(PARAM)	\
class PARAM : public Computable {	\
private:				\
	double compute() override;			\
public:					\
	const int order;				\
	PARAM (const Knot &knot, int order, const std::string &name) : Computable(knot, name), order(order) {} \
}

/***********************************************************************/

namespace KE {
 
namespace GL {

class KnotSurface;

}

namespace ThreeD {

class Knot : public seifert_base {

public:
	class Computable {

	public:
		const Knot &knot;
		const std::string name;

	private:
		bool ready;
		double internalValue;

	protected:
		virtual double compute() = 0;

	public:
		Computable(const Knot &knot, const std::string &name) : knot(knot), name(name), ready(false) {}
		virtual ~Computable() {}

		double value() {
			if (!this->ready) {
				this->internalValue = this->compute();
				this->ready = true;
			}

			return this->internalValue;
		}

		bool isReady() {return this->ready;}
		void invalidate() {this->ready = 0;}
	};

protected:
	addParameterClass(prmLength);
	addParameterClass(prmEnergy);

	class AverageCrossingNumber : public Computable {

	public:
		const bool withSign;

	private:
		double compute() override;

	public:
		AverageCrossingNumber(const Knot &knot, bool withSign);
	};

	class VassilievInvariant : public Computable {

	public:
		const int order;

	private:
		double compute() override;

	public:
		VassilievInvariant(const Knot &knot, int order);
	};

	addParameterClass(prmAen);
	addParameterClass(prmExperimental);
	addParameterClass(prmSingular);
	addParameterClassWithOrder(prmExperimental2);

public:
	std::string caption;
	std::shared_ptr<Computable> length;
	std::vector<std::shared_ptr<Computable>> computables;

private:
	std::vector<Point> points;

private:
	mutable std::vector<double> _len_table;

	void create_depend();
	void clear_depend();

	const std::vector<double> &len_table() const;

	bool noMorePoints (const double*) const override;
	double minDist (const double*) const override;

public:
	Knot(std::istream &is);
	Knot(diagram*, int, int);

	bool isEmpty();
	void decreaseEnergy();
	void setLength(double);
	void center();
	std::size_t numberOfPoints() const { return this->points.size(); }
	void normalize(std::size_t numberOfPoints);
	void getGradient (const double*, double*) const override;

	void save(std::ostream &os, const double matrix[3][3]) const;

	friend class GL::KnotSurface;

private:
	std::size_t next(std::size_t) const;
	std::size_t prev(std::size_t) const;

private:
	Knot(const Knot&) = delete;
	Knot& operator = (const Knot&) = delete;
};

/***********************************************************************/

inline std::size_t Knot::next(std::size_t index) const {
	return index == this->points.size() - 1 ? 0 : index + 1;
}

inline std::size_t Knot::prev(std::size_t index) const {
	return index ? index - 1 : this->points.size() - 1;
}

inline bool Knot::isEmpty() {
	return this->points.empty();
}

}}

#endif /* __KNOT_H__ */

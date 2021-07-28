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

namespace KE { namespace ThreeD {

class knot_surface;

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

protected:
	std::string caption;
	std::vector<Point> points;

	std::vector<std::shared_ptr<Computable>> computables;
	std::shared_ptr<Computable> length;

private:
	mutable std::vector<double> _len_table;

	void create_depend();
	void clear_depend();

	const std::vector<double> &len_table() const;

	bool noMorePoints (const double*);
	double minDist (const double*);

protected:
	Knot();
	Knot(diagram*, int, int);

	std::size_t next(std::size_t) const;
	std::size_t prev(std::size_t) const;

	bool isEmpty();
	void decreaseEnergy();
	void setLength (double);
	void center();
	void normalize (int);
	void getGradient (const double*, double*);

	friend std::istream & operator >> (std::istream &, Knot *);
	friend std::ostream & operator << (std::ostream &, Knot *);

	friend class knot_surface;

private:
	Knot (const Knot&);
	Knot& operator = (const Knot&);
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

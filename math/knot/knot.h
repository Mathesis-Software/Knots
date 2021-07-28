#ifndef __KNOT_H__
#define __KNOT_H__

#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>

#include "point.h"
#include "parameter.h"
#include "../seifert/seifert.h"

class diagram;
class Knot;
class knot_surface;

/***********************************************************************/

#define	addParameterClass(PARAM)	\
class PARAM : public parameter {	\
private:				\
	double compute() override;			\
public:					\
	PARAM (const Knot &knot, const std::string &name) : parameter(knot, name) {} \
}

#define	addParameterClassWithOrder(PARAM)	\
class PARAM : public parameter {	\
private:				\
	double compute() override;			\
public:					\
	const int order;				\
	PARAM (const Knot &knot, int order, const std::string &name) : parameter(knot, name), order(order) {} \
}

/***********************************************************************/

class Knot : public seifert_base {

protected:
	std::string caption;
	std::vector<point> points;

	std::list<std::shared_ptr<parameter>> parameterList;
	std::shared_ptr<parameter> Length;

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

	addParameterClass(prmLength);
	addParameterClass(prmEnergy);

	class AverageCrossingNumber : public parameter {

	public:
		const bool withSign;

	private:
		double compute() override;

	public:
		AverageCrossingNumber(const Knot &knot, bool withSign);
	};

	addParameterClass(prmAen);
	addParameterClassWithOrder(prmKI);
	addParameterClass(prmExperimental);
	addParameterClass(prmSingular);
	addParameterClassWithOrder(prmExperimental2);

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

#endif /* __KNOT_H__ */

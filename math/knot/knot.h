#ifndef __KNOT_H__
#define __KNOT_H__

#include <iostream>
#include <list>
#include <memory>
#include <string>
#include <vector>

#include "../point.h"
#include "../headers/parameter.h"
#include "../seifert/seifert.h"

class diagram;
class knot;
class knot_surface;

/***********************************************************************/

#define	addParameterClass(PARAM)	\
class PARAM : public parameter {	\
private:				\
	knot *Parent;				\
	void compute();			\
public:					\
	PARAM (knot *p, const char *n)	\
		: parameter (n)			\
	{Parent = p;};			\
}

#define	addParameterClassWithOrder(PARAM)	\
class PARAM : public parameter {	\
private:				\
	knot *Parent;				\
	int order;				\
	void compute();			\
public:					\
	PARAM (knot *p, int o, const char *n)	\
		: parameter (n)			\
	{Parent = p; order = o;};		\
}

/***********************************************************************/

class knot : public seifert_base {

protected:
	std::string caption;
	std::size_t length;
	std::vector<point> points;

	std::list<std::shared_ptr<parameter>> parameterList;
	std::shared_ptr<parameter> Length;

private:

	double *len_table;

	void create_depend();
	void clear_depend();

	void create_len_table();
	void delete_len_table();

	bool noMorePoints (const double*);
	double minDist (const double*);

protected:

	knot();
	knot (diagram*, int, int);
	~knot();

	std::size_t next(std::size_t);
	std::size_t prev(std::size_t);

	bool isEmpty();
	void decreaseEnergy();
	void setLength (double);
	void center();
	void normalize (int);
	void getGradient (const double*, double*);

	friend std::istream & operator >> (std::istream &, knot *);
	friend std::ostream & operator << (std::ostream &, knot *);

	addParameterClass(prmLength);
	addParameterClass(prmEnergy);
	addParameterClass(prmAcn);
	addParameterClass(prmSAcn);
	addParameterClass(prmAen);
	addParameterClassWithOrder(prmKI);
	addParameterClass(prmExperimental);
	addParameterClass(prmSingular);
	addParameterClassWithOrder(prmExperimental2);

	friend class knot_surface;

private:

	knot (const knot&);
	knot& operator = (const knot&);
};

/***********************************************************************/

inline std::size_t knot::next(std::size_t index) {
	return (index == length - 1) ? 0 : index + 1;
}

inline std::size_t knot::prev(std::size_t index) {
	return index ? index - 1 : length - 1;
}

inline bool knot::isEmpty() {
	return length == 0;
}

#endif /* __KNOT_H__ */

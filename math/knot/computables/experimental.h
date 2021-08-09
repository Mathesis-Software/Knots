#ifndef __EXPERIMENTAL_H__
#define __EXPERIMENTAL_H__

#include "computable.h"

namespace KE { namespace ThreeD {

class Knot;

namespace Computables {

class Experimental : public Computable {

private:
	double compute() override;

public:
	Experimental(const Knot &knot);
};

class Singular : public Computable {

private:
	double compute() override;

public:
	Singular(const Knot &knot);
};

class Experimental2 : public Computable {

public:
	const int order;

private:
	double compute() override;

public:
	Experimental2(const Knot &knot, int order);
};

}}}

#endif /* __EXPERIMENTAL_H__ */

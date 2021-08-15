#ifndef __EXPERIMENTAL_H__
#define __EXPERIMENTAL_H__

#include "computable.h"

namespace KE { namespace ThreeD {

class Knot;

namespace Computables {

class Experimental : public Computable {

private:
	double compute(const Knot::Snapshot &snapshot) override;

public:
	Experimental(const KnotWrapper &knot);
};

class Singular : public Computable {

private:
	double compute(const Knot::Snapshot &snapshot) override;

public:
	Singular(const KnotWrapper &knot);
};

class Experimental2 : public Computable {

public:
	const int order;

private:
	double compute(const Knot::Snapshot &snapshot) override;

public:
	Experimental2(const KnotWrapper &knot, int order);
};

}}}

#endif /* __EXPERIMENTAL_H__ */

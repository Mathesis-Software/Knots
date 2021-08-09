#ifndef __COMPUTABLES_H__
#define __COMPUTABLES_H__

#include "computable.h"

namespace KE { namespace ThreeD {

class Knot;

namespace Computables {

class AverageCrossingNumber : public Computable {

public:
	const bool withSign;

private:
	double compute() override;

public:
	AverageCrossingNumber(const Knot &knot, bool withSign);
};

class AverageExtremumNumber : public Computable {

private:
	double compute() override;

public:
	AverageExtremumNumber(const Knot &knot);
};

class MoebiusEnergy : public Computable {

private:
	double compute() override;

public:
	MoebiusEnergy(const Knot &knot);
};

class VassilievInvariant : public Computable {

public:
	const int order;

private:
	double compute() override;

public:
	VassilievInvariant(const Knot &knot, int order);
};

}}}

#endif /* __COMPUTABLES_H__ */

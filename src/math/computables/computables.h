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
	double compute(const Knot::Snapshot &snapshot) override;

public:
	AverageCrossingNumber(const KnotWrapper &knot, bool withSign);
};

class AverageExtremumNumber : public Computable {

private:
	double compute(const Knot::Snapshot &snapshot) override;

public:
	AverageExtremumNumber(const KnotWrapper &knot);
};

class MoebiusEnergy : public Computable {

private:
	double compute(const Knot::Snapshot &snapshot) override;

public:
	MoebiusEnergy(const KnotWrapper &knot);
};

class VassilievInvariant : public Computable {

public:
	const int order;

private:
	double compute(const Knot::Snapshot &snapshot) override;

public:
	VassilievInvariant(const KnotWrapper &knot, int order);
};

}}}

#endif /* __COMPUTABLES_H__ */

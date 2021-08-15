#ifndef __COMPUTABLE_H__
#define __COMPUTABLE_H__

#include "../knot/Knot.h"

namespace KE { namespace ThreeD {

class KnotWrapper;

namespace Computables {

class Computable {

public:
	const std::string name;

private:
	const KnotWrapper &knot;
	std::shared_ptr<Knot::Snapshot> snapshot;
	double _value;

protected:
	virtual double compute(const Knot::Snapshot &snapshot) = 0;

public:
	Computable(const KnotWrapper &knot, const std::string &name) : name(name), knot(knot) {}
	virtual ~Computable() {}

	double value();
};

}}}

#endif /* __COMPUTABLE_H__ */

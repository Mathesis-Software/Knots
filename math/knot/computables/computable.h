#ifndef __COMPUTABLE_H__
#define __COMPUTABLE_H__

#include <string>

namespace KE { namespace ThreeD {

class Knot;

namespace Computables {

class Computable {

public:
	const Knot &knot;
	const std::string name;

private:
	std::size_t generation;
	double internalValue;

protected:
	virtual double compute() = 0;

public:
	Computable(const Knot &knot, const std::string &name) : knot(knot), name(name), generation(0) {}
	virtual ~Computable() {}

	double value();
};

}}}

#endif /* __COMPUTABLE_H__ */

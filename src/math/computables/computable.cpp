#include "computable.h"
#include "../knot/Knot.h"

namespace KE { namespace ThreeD { namespace Computables {

double Computable::value() {
	const std::size_t gen = this->knot.generation();
	if (this->generation < gen) {
		this->internalValue = this->compute();
		this->generation = gen;
	}

	return this->internalValue;
}

}}}

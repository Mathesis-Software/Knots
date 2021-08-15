#include "computable.h"
#include "../knotWrapper/KnotWrapper.h"

namespace KE { namespace ThreeD { namespace Computables {

double Computable::value() {
	if (!this->snapshot || this->snapshot->isObsolete()) {
		this->snapshot = std::make_shared<Knot::Snapshot>(this->knot.snapshot());
		this->_value = this->compute(*this->snapshot);
	}

	return this->_value;
}

}}}

#ifndef __LENGTH_H__
#define __LENGTH_H__

#include "computable.h"

namespace KE { namespace ThreeD {

class Knot;

namespace Computables {

class Length : public Computable {

private:
	double compute() override;

public:
	Length(const Knot &knot);
};

}}}

#endif /* __LENGTH_H__ */

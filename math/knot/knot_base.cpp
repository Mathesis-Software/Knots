#include "knot.h"

namespace KE { namespace ThreeD {

void Knot::create_depend() {
  this->length = std::make_shared<prmLength>(*this, "Length");
  this->computables.push_back(std::make_shared<prmEnergy>(*this, "Moebius energy"));
  this->computables.push_back(std::make_shared<AverageCrossingNumber>(*this, false));
  this->computables.push_back(std::make_shared<AverageCrossingNumber>(*this, true));
  this->computables.push_back(std::make_shared<prmAen>(*this, "Average extremum number"));
	for (int order = 2; order <= 4; ++order) {
		this->computables.push_back(std::make_shared<VassilievInvariant>(*this, order));
	}
  this->computables.push_back(std::make_shared<prmExperimental>(*this, "Experimental"));
  this->computables.push_back(std::make_shared<prmSingular>(*this, "Singular"));
//  this->computables.push_back(std::make_shared<prmExperimental>2(*this, 2, "Experimental 2"));
//  this->computables.push_back(std::make_shared<prmExperimental>2(*this, 3, "Experimental 3"));
//  this->computables.push_back(std::make_shared<prmExperimental>2(*this, 4, "Experimental 4"));
}

void Knot::clear_depend() {
  this->_len_table.clear();

  this->length->invalidate();
  for (auto computable : this->computables) {
    computable->invalidate();
  }
}

}}

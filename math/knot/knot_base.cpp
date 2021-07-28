#include "knot.h"

Knot::Knot() : caption("New Knot") {
  create_depend();
}

void Knot::create_depend() {
  Length = std::make_shared<prmLength>(*this, "Length");
  parameterList.push_back(std::make_shared<prmEnergy>(*this, "Moebius energy"));
  parameterList.push_back(std::make_shared<AverageCrossingNumber>(*this, false));
  parameterList.push_back(std::make_shared<AverageCrossingNumber>(*this, true));
  parameterList.push_back(std::make_shared<prmAen>(*this, "Average extremum number"));
	for (int order = 2; order <= 4; ++order) {
		parameterList.push_back(std::make_shared<VassilievInvariant>(*this, order));
	}
  parameterList.push_back(std::make_shared<prmExperimental>(*this, "Experimental"));
  parameterList.push_back(std::make_shared<prmSingular>(*this, "Singular"));
//  parameterList.push_back(std::make_shared<prmExperimental>2(*this, 2, "Experimental 2"));
//  parameterList.push_back(std::make_shared<prmExperimental>2(*this, 3, "Experimental 3"));
//  parameterList.push_back(std::make_shared<prmExperimental>2(*this, 4, "Experimental 4"));
}

void Knot::clear_depend() {
  this->_len_table.clear();

  Length->destroy ();
  for (auto param : this->parameterList) {
    param->destroy();
  }
}

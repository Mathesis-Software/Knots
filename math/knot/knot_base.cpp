#include "knot.h"

knot::knot() : caption("New Knot") {
  create_depend();
}

void knot::create_depend() {
  Length = std::make_shared<prmLength> (this, "Length");
  parameterList.push_back(std::make_shared<prmEnergy>(this, "Moebius energy"));
  parameterList.push_back(std::make_shared<prmAcn>(this, "Average crossing number"));
  parameterList.push_back(std::make_shared<prmSAcn>(this, "Signed average crossing number"));
  parameterList.push_back(std::make_shared<prmAen>(this, "Average extremum number"));
  parameterList.push_back(std::make_shared<prmKI>(this, 2, "Order 2 Vassiliev invariant"));
  parameterList.push_back(std::make_shared<prmKI>(this, 3, "Order 3 Vassiliev invariant"));
  parameterList.push_back(std::make_shared<prmKI>(this, 4, "Order 4 Vassiliev invariant"));
//  parameterList.push_back(std::make_shared<prmKI>(this, 5, "Order 5 Vassiliev invariant"));
//  parameterList.push_back(std::make_shared<prmKI>(this, 6, "Order 6 Vassiliev invariant"));
//  parameterList.push_back(std::make_shared<prmKI>(this, 7, "Order 7 Vassiliev invariant"));
//  parameterList.push_back(std::make_shared<prmKI>(this, 8, "Order 8 Vassiliev invariant"));
//  parameterList.push_back(std::make_shared<prmKI>(this, 9, "Order 9 Vassiliev invariant"));
//  parameterList.push_back(std::make_shared<prmKI>(this, 10, "Order 10 Vassiliev invariant"));
//  parameterList.push_back(std::make_shared<prmKI>(this, 15, "Order 15 Vassiliev invariant"));
//  parameterList.push_back(std::make_shared<prmKI>(this, 20, "Order 20 Vassiliev invariant"));
  parameterList.push_back(std::make_shared<prmExperimental>(this, "Experimental"));
//  parameterList.push_back(std::make_shared<prmSingular>(this, "Singular"));
//  parameterList.push_back(std::make_shared<prmExperimental>2(this, 2, "Experimental 2"));
//  parameterList.push_back(std::make_shared<prmExperimental>2(this, 3, "Experimental 3"));
//  parameterList.push_back(std::make_shared<prmExperimental>2(this, 4, "Experimental 4"));
}

void knot::clear_depend() {
  this->len_table.clear();

  Length->destroy ();
  for (auto param : this->parameterList) {
    param->destroy();
  }
}

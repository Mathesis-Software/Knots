#include "knot.h"

knot::knot (void)
{
  Caption = new char [256];
  strcpy (Caption, "New Knot");

  length = 0;
  points = NULL;

  create_depend ();
}

void knot::create_depend (void)
{
  len_table = NULL;

  parameterList = new std::list<parameter*>;
  Length = new prmLength (this, "Length");
  parameterList -> push_back (new prmEnergy (this, "Moebius energy"));
  parameterList -> push_back (new prmAcn (this, "Average crossing number"));
  parameterList -> push_back (new prmSAcn (this, "Signed average crossing number"));
  parameterList -> push_back (new prmAen (this, "Average extremum number"));
  parameterList -> push_back (new prmKI (this, 2, "Order 2 Vassiliev invariant"));
  parameterList -> push_back (new prmKI (this, 3, "Order 3 Vassiliev invariant"));
  parameterList -> push_back (new prmKI (this, 4, "Order 4 Vassiliev invariant"));
//  parameterList -> push_back (new prmKI (this, 5, "Order 5 Vassiliev invariant"));
//  parameterList -> push_back (new prmKI (this, 6, "Order 6 Vassiliev invariant"));
//  parameterList -> push_back (new prmKI (this, 7, "Order 7 Vassiliev invariant"));
//  parameterList -> push_back (new prmKI (this, 8, "Order 8 Vassiliev invariant"));
//  parameterList -> push_back (new prmKI (this, 9, "Order 9 Vassiliev invariant"));
//  parameterList -> push_back (new prmKI (this, 10, "Order 10 Vassiliev invariant"));
//  parameterList -> push_back (new prmKI (this, 15, "Order 15 Vassiliev invariant"));
//  parameterList -> push_back (new prmKI (this, 20, "Order 20 Vassiliev invariant"));
  parameterList -> push_back (new prmExperimental (this, "Experimental"));
//  parameterList -> push_back (new prmSingular (this, "Singular"));
//  parameterList -> push_back (new prmExperimental2 (this, 2, "Experimental 2"));
//  parameterList -> push_back (new prmExperimental2 (this, 3, "Experimental 3"));
//  parameterList -> push_back (new prmExperimental2 (this, 4, "Experimental 4"));
}

void knot::clear_depend (void)
{
  delete_len_table ();

  Length -> destroy ();
  for (std::list<parameter*>::iterator it = parameterList -> begin ();
       it != parameterList -> end ();
       it++)
    (*it) -> destroy ();
}

knot::~knot (void)
{
  delete[] Caption;

  delete_len_table ();
  
  for (int i = 0; i < length; i++)
    delete[] points [i];

  delete[] points;

  for (std::list<parameter*>::iterator it = parameterList -> begin ();
       it != parameterList -> end ();
       it++)
    delete (*it);

  delete parameterList;
}

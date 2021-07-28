#ifndef __SEIFERT_H__
#define __SEIFERT_H__

#include <stdlib.h>

#include "seifert_surface.h"

class seifert;
class seifert_list;
class seifert_ord;
class seifert_base;

namespace KE { namespace ThreeD {

class Knot;

}}

class seifert_base {

protected:

  seifert_base() {}
  virtual ~seifert_base() {}
  virtual bool noMorePoints(const double*) const = 0;
  virtual double minDist(const double*) const = 0;

  friend class seifert;
  friend class seifert_list;
};

class seifert_list {

private:

  seifert *basepoint, *value;
  seifert_list *prev, *next;
  bool label;

  void insert(seifert*);
  void insert_after(seifert*);

  seifert_list(seifert*, seifert*);
  ~seifert_list();

  friend class seifert;
  friend class KE::GL::SeifertSurface;
};

class seifert_ord {

private:

  seifert *value;
  seifert_ord *prev, *next;

  seifert_ord *insert(seifert*);

  seifert_ord(seifert*);
  ~seifert_ord();

  friend class seifert;
  friend class KE::GL::SeifertSurface;
};

class seifert {

private:

  double coord[3], gradient[3];
  double localEps;
  seifert_list *neighborhood;
  seifert_ord *sord;
  const KE::ThreeD::Knot &base;

  void searchForNeighbor();
  void checkNeighborhood();
  void addPoint(double, double, double);
  void addPoint60(double, double, double);
  seifert_list *hasNeighbor(seifert*);
  void markUsed(seifert*, seifert*);
  void correction_local();
  void correction();

public:

  seifert(const double, const double, const double,
          const KE::ThreeD::Knot &base, seifert* = nullptr);
  ~seifert();

  friend class seifert_ord;
  friend class seifert_list;
  friend class KE::GL::SeifertSurface;
};

#endif /* __SEIFERT_H__ */

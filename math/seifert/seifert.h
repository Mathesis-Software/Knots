#ifndef __SEIFERT_H__
#define __SEIFERT_H__

#include <stdlib.h>

#include "seifert_surface.h"

class seifert;
class seifert_list;
class seifert_ord;
class seifert_base;

class seifert_base {

protected:

  seifert_base (void) {};
  virtual ~seifert_base (void) {};
  virtual void getGradient (const double*, double*) = 0;
  virtual bool noMorePoints (const double*) = 0;
  virtual double minDist (const double*) = 0;

  friend class seifert;
  friend class seifert_list;
};

class seifert_list {

private:

  seifert *basepoint, *value;
  seifert_list *prev, *next;
  bool label;

  void insert (seifert*);
  void insert_after (seifert*);

  seifert_list (seifert*, seifert*);
  ~seifert_list (void);

  friend class seifert;
  friend class seifert_surface;
};

class seifert_ord {

private:

  seifert *value;
  seifert_ord *prev, *next;

  seifert_ord *insert (seifert*);

  seifert_ord (seifert*);
  ~seifert_ord (void);

  friend class seifert;
  friend class seifert_surface;
};

class seifert {

private:

  double coord [3], gradient [3];
  double localEps;
  seifert_list *neighborhood;
  seifert_ord *sord;
  seifert_base *base;

  void searchForNeighbor (void);
  void checkNeighborhood (void);
  void addPoint (double, double, double);
  void addPoint60 (double, double, double);
  seifert_list *hasNeighbor (seifert*);
  void markUsed (seifert*, seifert*);
  void correction_local (void);
  void correction (void);

public:

  seifert (const double, const double, const double,
           seifert_base* = NULL, seifert* = NULL);
  ~seifert (void);

  friend class seifert_ord;
  friend class seifert_list;
  friend class seifert_surface;
};

#endif /* __SEIFERT_H__ */

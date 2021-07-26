#ifndef __DIAGRAM_H__
#define __DIAGRAM_H__

#include <iostream>

#include "../knot/knot.h"
#include "vertex.h"

class diagram {

friend class knot;

protected:

  vertex *base;
  char *Caption;

  void addVertex (vertex*, int, int);
  void removeVertex (vertex*);
  void moveVertex (vertex*, int, int);
  bool tryChangeCrossing (vertex*, vertex*);
  bool isCrossing (vertex*, vertex*);
  void shift (int x, int y);
  bool simplify (int depth);
  void clear (void);
  int length (void);

private:

  int numByV (vertex*);
  vertex *vByNum (int);

  bool tryAddCrossing (vertex*, vertex*);
  bool tryRemoveCrossing (vertex*, vertex*);

  void order (void);

public:

  diagram (void);
  ~diagram (void);

  bool isEmpty (void);
  
  friend std::istream & operator >> (std::istream &, diagram *);
  friend std::ostream & operator << (std::ostream &, diagram *);

private:

  diagram (const diagram&);
  diagram& operator = (const diagram&);
};

inline bool diagram::isEmpty (void)
{
  return (!base);
}

#endif /* __DIAGRAM_H__ */

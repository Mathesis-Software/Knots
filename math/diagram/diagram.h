#ifndef __DIAGRAM_H__
#define __DIAGRAM_H__

#include <iostream>
#include <string>

#include "../knot/knot.h"
#include "vertex.h"

class diagram {

friend class knot;

protected:
	std::string caption;
  vertex *base;

  void addVertex(vertex*, int, int);
  void removeVertex(vertex*);
  void moveVertex(vertex*, int, int);
  bool tryChangeCrossing(vertex*, vertex*);
  bool isCrossing(vertex*, vertex*);
  void shift(int x, int y);
  bool simplify(int depth);
  void clear();
  int length();

private:
  int numByV(vertex*);
  vertex *vByNum(int);

  bool tryAddCrossing(vertex*, vertex*);
  bool tryRemoveCrossing(vertex*, vertex*);

  void order();

public:
  diagram();
  ~diagram();

  bool isEmpty();
  
  friend std::istream & operator>>(std::istream &, diagram *);
  friend std::ostream & operator<<(std::ostream &, diagram *);

private:
  diagram(const diagram&) = delete;
  diagram& operator=(const diagram&) = delete;
};

inline bool diagram::isEmpty() {
  return this->base == nullptr;
}

#endif /* __DIAGRAM_H__ */

#ifndef __DIAGRAM_H__
#define __DIAGRAM_H__

#include <iostream>
#include <string>

#include "vertex.h"

namespace KE {

namespace ThreeD {

class Knot;

}

namespace TwoD {

class Diagram {

friend class ThreeD::Knot;

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
	Diagram();
	virtual ~Diagram();

	bool isEmpty();
	
	friend std::istream & operator>>(std::istream &, Diagram *);
	friend std::ostream & operator<<(std::ostream &, Diagram *);

private:
	Diagram(const Diagram&) = delete;
	Diagram& operator=(const Diagram&) = delete;
};

inline bool Diagram::isEmpty() {
	return this->base == nullptr;
}

}}

#endif /* __DIAGRAM_H__ */

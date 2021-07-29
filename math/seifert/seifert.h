#ifndef __SEIFERT_H__
#define __SEIFERT_H__

#include "seifert_surface.h"

class seifert;
class seifert_list;
class seifert_ord;

namespace KE { namespace ThreeD {

class Knot;

}}

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
	const KE::ThreeD::Knot &base;
	const KE::ThreeD::Point point;
	const KE::ThreeD::Vector gradient;

	double localEps;
	seifert_list *neighborhood;
	seifert_ord *sord;

	void searchForNeighbor();
	void checkNeighborhood();
	void addPoint(const KE::ThreeD::Vector &direction);
	void addPoint60(const KE::ThreeD::Vector &direction);
	seifert_list *hasNeighbor(seifert*);
	void markUsed(seifert*, seifert*);
	void correction_local();
	void correction();

public:
	seifert(const KE::ThreeD::Knot &base, const KE::ThreeD::Point &point, seifert* = nullptr);
	~seifert();

	friend class seifert_ord;
	friend class seifert_list;
	friend class KE::GL::SeifertSurface;
};

#endif /* __SEIFERT_H__ */

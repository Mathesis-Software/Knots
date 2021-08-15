#ifndef __SEIFERT_SURFACE_H__
#define __SEIFERT_SURFACE_H__

#include "../knot/Knot.h"
#include "../../gl/surface/surface.h"

class seifert;

namespace KE { namespace GL {

class SeifertSurface : public Surface {

public:
	static ThreeD::Vector gradient(const ThreeD::Point &point, const ThreeD::Knot::Snapshot &snapshot);

private:
	std::shared_ptr<ThreeD::Knot::Snapshot> stored;

private:
	const ThreeD::Knot &base;
	const ThreeD::Point &startPoint;

	void addTriangles(seifert *s);
	void calculate() override;

public:
	SeifertSurface(const ThreeD::Knot &base, const ThreeD::Point &startPoint);

	bool destroy(bool force);
};

}}

#endif /* __SEIFERT_SURFACE_H__ */

#ifndef __SEIFERT_SURFACE_H__
#define __SEIFERT_SURFACE_H__

#include "../knotWrapper/KnotWrapper.h"
#include "../../gl/surface/Surface.h"

class seifert;

namespace KE { namespace GL {

class SeifertSurface : public Surface {

public:
	static ThreeD::Vector gradient(const ThreeD::Point &point, const ThreeD::Knot::Snapshot &snapshot);

private:
	std::shared_ptr<ThreeD::Knot::Snapshot> stored;

private:
	const ThreeD::KnotWrapper &base;
	const ThreeD::Point &startPoint;

	void addTriangles(seifert *s);
	void calculate() override;

public:
	SeifertSurface(const ThreeD::KnotWrapper &base, const ThreeD::Point &startPoint);

	const Color &frontColor() const override;
	const Color &backColor() const override;

	bool destroy(bool force);
};

}}

#endif /* __SEIFERT_SURFACE_H__ */

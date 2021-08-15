#ifndef __KNOT_SURFACE_H__
#define __KNOT_SURFACE_H__

#include <vector>

#include "../knotWrapper/KnotWrapper.h"
#include "../../gl/surface/Surface.h"

namespace KE { namespace GL {

class KnotSurface : public Surface {

private:
	const ThreeD::KnotWrapper &knot;
	double thickness;
	std::vector<double> sines;
	std::vector<double> cosines;
	std::shared_ptr<ThreeD::Knot::Snapshot> stored;

public:
	KnotSurface(const ThreeD::KnotWrapper &knot, double thickness, std::size_t numberOfPointsOnMeridian);
	void calculate() override;
	bool destroy(bool force);
	void setThickness(double thickness);
	void setNumberOfPointsOnMeridian(std::size_t numberOfPointsOnMeridian);

	const Color &getFrontColor() const override;
	const Color &getBackColor() const override;

private:
	KnotSurface(const KnotSurface&) = delete;
	KnotSurface& operator = (const KnotSurface&) = delete;
};

}}

#endif /* __KNOT_SURFACE_H__ */

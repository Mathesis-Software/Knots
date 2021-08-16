#ifndef __KNOT_SURFACE_H__
#define __KNOT_SURFACE_H__

#include <vector>

#include "../knotWrapper/KnotWrapper.h"
#include "../../gl/surface/Surface.h"

namespace KE { namespace GL {

class KnotSurface : public Surface {

private:
	const ThreeD::KnotWrapper &knot;
	std::vector<double> sines;
	std::vector<double> cosines;
	std::shared_ptr<ThreeD::Knot::Snapshot> stored;

public:
	KnotSurface(const ThreeD::KnotWrapper &knot, std::size_t numberOfPointsOnMeridian);
	void setNumberOfPointsOnMeridian(std::size_t numberOfPointsOnMeridian);

	double thickness() const;
	const Color &frontColor() const override;
	const Color &backColor() const override;

	bool destroy(bool force) override;
	bool isVisible() const override;

private:
	void calculate() override;

private:
	KnotSurface(const KnotSurface&) = delete;
	KnotSurface& operator = (const KnotSurface&) = delete;
};

}}

#endif /* __KNOT_SURFACE_H__ */

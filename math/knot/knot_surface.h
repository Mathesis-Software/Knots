#ifndef __KNOT_SURFACE_H__
#define __KNOT_SURFACE_H__

#include <vector>

#include "knot.h"
#include "../../gl/surface/surface.h"

namespace KE { namespace GL {

class KnotSurface : public Surface {

private:
	const ThreeD::Knot &knot;
  double thickness;
	std::vector<double> sines;
	std::vector<double> cosines;
	std::shared_ptr<ThreeD::Knot::Snapshot> stored;

public:
  KnotSurface(const ThreeD::Knot &knot, double thickness, std::size_t pointsOnCircle);
  void calculate() override;
  bool destroy(bool force);
  void setThickness(double thickness);
	void setNumberOfPointsOnCircle(std::size_t pointsOnCircle);

private:
	KnotSurface(const KnotSurface&) = delete;
	KnotSurface& operator = (const KnotSurface&) = delete;
};

}}

#endif /* __KNOT_SURFACE_H__ */

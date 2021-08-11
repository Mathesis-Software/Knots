#include <math.h>

#include "Knot.h"

namespace KE { namespace ThreeD {

double Knot::minDist(const Point &point) const {
  double md2 = 10000.0;

	const auto points = this->points();
  for (std::size_t i = 0; i < points.size(); i++) {
		const Vector x(point, points[i]);
		const Vector r(points[i], points[points.next(i)]);
		const double xr = x.scalar_product(r);
		const double x2 = x.square();
		const double r2 = r.square();
    const double tau = - xr / r2;

    if (tau < 0.0) {
      if (md2 > x2) {
        md2 = x2;
			}
      continue;
    }

    if (tau > 1.0) {
      if (md2 > x2 + r2 + 2 * xr) {
        md2 = x2 + r2 + 2 * xr;
			}
      continue;
    }

    if (md2 > x2 + tau * xr) {
      md2 = x2 + tau * xr;
		}
  }

  return sqrt(md2);
}

Vector Knot::seifertGradient(const Point &point) const {
	Vector gradient(0.0, 0.0, 0.0);

	const auto points = this->points();
  for (std::size_t i = 0; i < points.size(); i++) {
		const Vector x(point, points[i]);
		const Vector r(points[i], points[points.next(i)]);

		const double xr = x.scalar_product(r);
		const double x2 = x.square();
		const double r2 = r.square();
		const double tau = - xr / r2;
		const double a2 = x2 + tau * xr;
    const double coeff = ((tau - 1) / sqrt (x2 + r2 + xr + xr) - tau / sqrt(x2)) / a2;

    gradient.x += (r.y * x.z - r.z * x.y) * coeff;
    gradient.y += (r.z * x.x - r.x * x.z) * coeff;
    gradient.z += (r.x * x.y - r.y * x.x) * coeff;
  }

  gradient.normalize();
	return gradient;
}

}}

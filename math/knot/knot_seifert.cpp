#include <math.h>

#include "knot.h"

namespace KE { namespace ThreeD {

bool Knot::noMorePoints(const double *point) const {
  return (point [0] * point [0] +
          point [1] * point [1] +
          point [2] * point [2] > 2.0);
}

double Knot::minDist(const double *point) const {
  double md2 = 10000.0, tau, r[3], x[3], xr, r2, x2;

  for (std::size_t i = 0; i < this->points.size(); i++) {
    x [0] = points[i].x - point [0];
    x [1] = points[i].y - point [1];
    x [2] = points[i].z - point [2];
    r [0] = points[next (i)].x - points[i].x;
    r [1] = points[next (i)].y - points[i].y;
    r [2] = points[next (i)].z - points[i].z;
    xr = x [0] * r [0] + x [1] * r [1] + x [2] * r [2];
    r2 = r [0] * r [0] + r [1] * r [1] + r [2] * r [2];
    x2 = x [0] * x [0] + x [1] * x [1] + x [2] * x [2];
    tau = - xr / r2;

    if (tau < 0.0)
    {
      if (md2 > x2)
        md2 = x2;
      continue;
    }

    if (tau > 1.0)
    {
      if (md2 > x2 + r2 + 2 * xr)
        md2 = x2 + r2 + 2 * xr;
      continue;
    }

    if (md2 > x2 + tau * xr)
      md2 = x2 + tau * xr;
  }

  return sqrt (md2);
}

void Knot::getGradient(const double *point, double *gradient) const {
	const Point pt(point[0], point[1], point[2]);
	const Vector gr = this->seifertGradient(pt);
  gradient [0] = gr.x;
  gradient [1] = gr.y;
  gradient [2] = gr.z;
}

Vector Knot::seifertGradient(const Point &point) const {
	Vector gradient(0.0, 0.0, 0.0);

  for (std::size_t i = 0; i < this->points.size(); i++) {
		const Vector x(point, this->points[i]);
		const Vector r(this->points[i], this->points[next(i)]);

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

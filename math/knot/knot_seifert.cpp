#include <math.h>

#include "knot.h"

bool Knot::noMorePoints(const double *point) {
  return (point [0] * point [0] +
          point [1] * point [1] +
          point [2] * point [2] > 2.0);
}

double Knot::minDist(const double *point) {
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

void Knot::getGradient(const double *point, double *gradient) {
  gradient [0] = 0.0;
  gradient [1] = 0.0;
  gradient [2] = 0.0;

  double x [3], r [3], xr, x2, r2, a2, tau, coeff;

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
    a2 = x2 + tau * xr;

    coeff = ( (tau - 1) / sqrt (x2 + r2 + xr + xr) -
              tau / sqrt (x2) ) / a2;

    gradient [0] += (r [1] * x [2] - r [2] * x [1]) * coeff;
    gradient [1] += (r [2] * x [0] - r [0] * x [2]) * coeff;
    gradient [2] += (r [0] * x [1] - r [1] * x [0]) * coeff;
  }

  double len = sqrt (gradient [0] * gradient [0] +
                     gradient [1] * gradient [1] +
	             gradient [2] * gradient [2]);
  gradient [0] /= len;
  gradient [1] /= len;
  gradient [2] /= len;
}

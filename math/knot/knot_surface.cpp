#include <cmath>

#include "knot_surface.h"
#include "knot.h"

namespace {

double SIN[9] =
	{0.0, M_SQRT1_2, 1.0, M_SQRT1_2, 0.0, -M_SQRT1_2, -1.0, -M_SQRT1_2, 0.0 };
double COS[9] =
	{1.0, M_SQRT1_2, 0.0, -M_SQRT1_2, -1.0, -M_SQRT1_2, 0.0, M_SQRT1_2, 1.0 };

}

namespace KE { namespace GL {

KnotSurface::KnotSurface(const ThreeD::Knot &knot, double thickness) : knot(knot), thickness(thickness) {
	stripped = 1;
	sides = Front;
}

void KnotSurface::calculate() {
	const auto &points = this->knot.points;

	if (points.empty()) {
		return;
	}

	destroy ();

	std::vector<ThreeD::Vector> normal1, normal2;
	std::vector<int> shift;

	/*
	Create normal vector table
	*/

	for (std::size_t i = 0; i < points.size(); ++i) {
		ThreeD::Vector v(points[this->knot.prev(i)], points[this->knot.next(i)]);
		v.normalize();

		if (fabs(v.y) < M_SQRT1_2) {
			ThreeD::Vector norm1(v.z, 0.0, - v.x);
			ThreeD::Vector norm2(- v.x * v.y, v.x * v.x + v.z * v.z, - v.y * v.z);
			norm1.normalize();
			norm2.normalize();
			normal1.push_back(norm1);
			normal2.push_back(norm2);
		} else {
			ThreeD::Vector norm1(0.0, - v.z, v.y);
			ThreeD::Vector norm2(v.y * v.y + v.z * v.z, - v.x * v.y, - v.x * v.z);
			norm1.normalize();
			norm2.normalize();
			normal1.push_back(norm1);
			normal2.push_back(norm2);
		}
	}

	/*
	Create shift table
	*/
	{
		double a, b;

		for (std::size_t i = 0; i < points.size(); ++i) {
			a = normal1[i].x * normal1[this->knot.next(i)].x
				+ normal1[i].y * normal1[this->knot.next(i)].y
				+ normal1[i].z * normal1[this->knot.next(i)].z;
			b = normal1[i].x * normal2[this->knot.next(i)].x
				+ normal1[i].y * normal2[this->knot.next(i)].y
				+ normal1[i].z * normal2[this->knot.next(i)].z;

			if (a > 0) {
				if (b > 0) {
					shift.push_back((b > a) ? 7 : 0);
				} else {
					shift.push_back((- b > a) ? 2 : 1);
				}
			} else {
				if (b > 0) {
					shift.push_back((b > - a) ? 6 : 5);
				} else {
					shift.push_back((b > a) ? 4 : 3);
				}
			}
		}
	}

	/*
	Create surface
	*/

	{
		int j, j1, j2;
		double n0, n1, n2;

		for (j = 0; j < 8; ++j) {
			j1 = j;
			j2 = (j + 1) & 0x7;

			for (std::size_t i = 0; i < points.size(); ++i) {
				n0 = normal1[i].x * SIN[j1] + normal2[i].x * COS[j1];
				n1 = normal1[i].y * SIN[j1] + normal2[i].y * COS[j1];
				n2 = normal1[i].z * SIN[j1] + normal2[i].z * COS[j1];

				addpoint(
					points[i].x + thickness * n0,
					points[i].y + thickness * n1,
					points[i].z + thickness * n2,
					n0, n1, n2
				);

				n0 = normal1[i].x * SIN[j2] + normal2[i].x * COS[j2];
				n1 = normal1[i].y * SIN[j2] + normal2[i].y * COS[j2];
				n2 = normal1[i].z * SIN[j2] + normal2[i].z * COS[j2];

				addpoint(
					points[i].x + thickness * n0,
					points[i].y + thickness * n1,
					points[i].z + thickness * n2,
					n0, n1, n2
				);

				j1 += shift[i];
				j2 += shift[i];
				j1 &= 0x7;
				j2 &= 0x7;
			}

			n0 = normal1[0].x * SIN[j1] + normal2[0].x * COS[j1];
			n1 = normal1[0].y * SIN[j1] + normal2[0].y * COS[j1];
			n2 = normal1[0].z * SIN[j1] + normal2[0].z * COS[j1];

			addpoint(
				points[0].x + thickness * n0,
				points[0].y + thickness * n1,
				points[0].z + thickness * n2,
				n0, n1, n2
			);

			n0 = normal1[0].x * SIN[j2] + normal2[0].x * COS[j2];
			n1 = normal1[0].y * SIN[j2] + normal2[0].y * COS[j2];
			n2 = normal1[0].z * SIN[j2] + normal2[0].z * COS[j2];

			addpoint(
				points[0].x + thickness * n0,
				points[0].y + thickness * n1,
				points[0].z + thickness * n2,
				n0, n1, n2
			);
		}
	}
}

}}

#include <cmath>

#include "knot_surface.h"
#include "knot.h"

namespace {

std::vector<double> sines_vector(std::size_t length) {
	std::vector<double> sines;
	for (std::size_t i = 0; i <= length; ++i) {
		sines.push_back(sin(2 * M_PI / length * i));
	}
	return sines;
}
std::vector<double> cosines_vector(std::size_t length) {
	std::vector<double> cosines;
	for (std::size_t i = 0; i <= length; ++i) {
		cosines.push_back(cos(2 * M_PI / length * i));
	}
	return cosines;
}

}

namespace KE { namespace GL {

KnotSurface::KnotSurface(const ThreeD::Knot &knot, double thickness, std::size_t pointsOnCircle) : knot(knot), thickness(thickness), sines(sines_vector(pointsOnCircle)), cosines(cosines_vector(pointsOnCircle)) {
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
	std::vector<std::size_t> shift;

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
	for (std::size_t i = 0; i < points.size(); ++i) {
		std::size_t best = 0;
		double diff = std::numeric_limits<double>::max();
		for (std::size_t rotation = 0; rotation < this->sines.size() - 1; ++rotation) {
			ThreeD::Vector no(
				normal1[this->knot.next(i)].x * this->cosines[rotation]	-
					normal2[this->knot.next(i)].x * this->sines[rotation],
				normal1[this->knot.next(i)].y * this->cosines[rotation]	-
					normal2[this->knot.next(i)].y * this->sines[rotation],
				normal1[this->knot.next(i)].z * this->cosines[rotation]	-
					normal2[this->knot.next(i)].z * this->sines[rotation]
			);
			no.normalize();
			const double value = 1 - normal1[i].scalar_product(no);
			if (value < diff) {
				diff = value;
				best = rotation;
			}
		}
		shift.push_back(best);
	}

	/*
	Create surface
	*/

	{
		int j, j1, j2;
		double n0, n1, n2;

		for (j = 0; j < this->sines.size() - 1; ++j) {
			j1 = j;
			j2 = (j + 1) % (this->sines.size() - 1);

			for (std::size_t i = 0; i < points.size(); ++i) {
				n0 = normal1[i].x * this->sines[j1] + normal2[i].x * this->cosines[j1];
				n1 = normal1[i].y * this->sines[j1] + normal2[i].y * this->cosines[j1];
				n2 = normal1[i].z * this->sines[j1] + normal2[i].z * this->cosines[j1];

				addpoint(
					points[i].x + thickness * n0,
					points[i].y + thickness * n1,
					points[i].z + thickness * n2,
					n0, n1, n2
				);

				n0 = normal1[i].x * this->sines[j2] + normal2[i].x * this->cosines[j2];
				n1 = normal1[i].y * this->sines[j2] + normal2[i].y * this->cosines[j2];
				n2 = normal1[i].z * this->sines[j2] + normal2[i].z * this->cosines[j2];

				addpoint(
					points[i].x + thickness * n0,
					points[i].y + thickness * n1,
					points[i].z + thickness * n2,
					n0, n1, n2
				);

				j1 += shift[i];
				j2 += shift[i];
				j1 %= this->sines.size() - 1;
				j2 %= this->sines.size() - 1;
			}

			n0 = normal1[0].x * this->sines[j1] + normal2[0].x * this->cosines[j1];
			n1 = normal1[0].y * this->sines[j1] + normal2[0].y * this->cosines[j1];
			n2 = normal1[0].z * this->sines[j1] + normal2[0].z * this->cosines[j1];

			addpoint(
				points[0].x + thickness * n0,
				points[0].y + thickness * n1,
				points[0].z + thickness * n2,
				n0, n1, n2
			);

			n0 = normal1[0].x * this->sines[j2] + normal2[0].x * this->cosines[j2];
			n1 = normal1[0].y * this->sines[j2] + normal2[0].y * this->cosines[j2];
			n2 = normal1[0].z * this->sines[j2] + normal2[0].z * this->cosines[j2];

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

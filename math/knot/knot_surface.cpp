#include <cmath>

#include "knot_surface.h"
#include "knot.h"

namespace KE { namespace GL {

KnotSurface::KnotSurface(const ThreeD::Knot &knot, double thickness, std::size_t pointsOnCircle) : knot(knot) {
	stripped = 1;
	sides = Front;
	this->setThickness(thickness);
	this->setNumberOfPointsOnCircle(pointsOnCircle);
}

void KnotSurface::setThickness(double thickness) {
	this->thickness = thickness;
	this->destroy();
}

void KnotSurface::setNumberOfPointsOnCircle(std::size_t pointsOnCircle) {
	if (this->sines.size() == pointsOnCircle + 1) {
		return;
	}

	this->sines.clear();
	this->cosines.clear();
	for (std::size_t i = 0; i <= pointsOnCircle; ++i) {
		this->sines.push_back(sin(2 * M_PI / pointsOnCircle * i));
		this->cosines.push_back(cos(2 * M_PI / pointsOnCircle * i));
	}
	this->destroy();
}

void KnotSurface::calculate() {
	const auto &points = this->knot.points;

	if (points.empty()) {
		return;
	}

	destroy ();

	std::vector<ThreeD::Vector> normal1, normal2;
	/* Creating normal vector table */
	for (std::size_t i = 0; i < points.size(); ++i) {
		ThreeD::Vector v(points[this->knot.prev(i)], points[this->knot.next(i)]);
		v.normalize();

		ThreeD::Vector norm1(0.0, - v.z, v.y);
		if (fabs(v.y) < fabs(v.x)) {
			norm1 = ThreeD::Vector(v.z, 0.0, - v.x);
		}
		ThreeD::Vector norm2 = v.vector_product(norm1);
		norm1.normalize();
		norm2.normalize();
		normal1.push_back(norm1);
		normal2.push_back(norm2);
	}

	std::vector<std::size_t> shift;
	/* Creating shift table */
	for (std::size_t i = 0; i < points.size(); ++i) {
		std::size_t best = 0;
		double diff = - std::numeric_limits<double>::max();
		for (std::size_t rotation = 0; rotation < this->sines.size() - 1; ++rotation) {
			const ThreeD::Vector no = ThreeD::Vector::linear(
				normal1[this->knot.next(i)], this->cosines[rotation],
				normal2[this->knot.next(i)], - this->sines[rotation]
			);
			const double value = normal1[i].scalar_product(no);
			if (value > diff) {
				diff = value;
				best = rotation;
			}
		}
		shift.push_back(best);
	}

	/* Creating surface */
	for (std::size_t j = 0; j < this->sines.size() - 1; ++j) {
		std::size_t j1 = j;
		std::size_t j2 = (j + 1) % (this->sines.size() - 1);

		for (std::size_t i = 0; i <= points.size(); ++i) {
			const std::size_t ix = i < points.size() ? i : 0;
			for (auto jx : {j1, j2}) {
				const ThreeD::Vector n = ThreeD::Vector::linear(
					normal1[ix], this->sines[jx],
					normal2[ix], this->cosines[jx]
				);
				ThreeD::Point new_point(points[ix]);
				new_point.move(n, thickness);
				addpoint(new_point, n);
			}

			j1 += shift[ix];
			j2 += shift[ix];
			j1 %= this->sines.size() - 1;
			j2 %= this->sines.size() - 1;
		}
	}
}

}}

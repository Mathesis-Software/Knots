#include <cmath>

#include "KnotSurface.h"

namespace KE { namespace GL {

KnotSurface::KnotSurface(const ThreeD::KnotWrapper &knot, double thickness, std::size_t numberOfPointsOnMeridian) : Surface(true, false), knot(knot) {
	this->setThickness(thickness);
	this->setNumberOfPointsOnMeridian(numberOfPointsOnMeridian);
}

namespace {

const Color white(255, 255, 255);

}

const Color &KnotSurface::getFrontColor() const {
	const auto saved = this->knot.knotColor;
	return saved ? *saved : white;
}

const Color &KnotSurface::getBackColor() const {
	return white;
}

void KnotSurface::setThickness(double thickness) {
	if (thickness != this->thickness) {
		this->thickness = thickness;
		this->destroy(true);
	}
}

void KnotSurface::setNumberOfPointsOnMeridian(std::size_t numberOfPointsOnMeridian) {
	if (this->sines.size() == numberOfPointsOnMeridian + 1) {
		return;
	}

	this->sines.clear();
	this->cosines.clear();
	for (std::size_t i = 0; i <= numberOfPointsOnMeridian; ++i) {
		this->sines.push_back(sin(2 * M_PI / numberOfPointsOnMeridian * i));
		this->cosines.push_back(cos(2 * M_PI / numberOfPointsOnMeridian * i));
	}
	this->destroy(true);
}

bool KnotSurface::destroy(bool force) {
	if (!force && this->stored && !this->stored->isObsolete()) {
		return false;
	}
	Surface::destroy();
	return true;
}

void KnotSurface::calculate() {
	const auto points = this->knot.snapshot();
	this->stored = std::make_shared<ThreeD::Knot::Snapshot>(points);

	if (points.size() == 0) {
		return;
	}

	std::vector<ThreeD::Vector> normal1, normal2;
	/* Creating normal vector table */
	for (std::size_t i = 0; i < points.size(); ++i) {
		ThreeD::Vector v(points[points.prev(i)], points[points.next(i)]);
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
	std::vector<double> scalars;
	/* Creating shift table */
	for (std::size_t i = 0; i < points.size(); ++i) {
		scalars.clear();
		for (std::size_t rotation = 0; rotation < this->sines.size() - 1; ++rotation) {
			const ThreeD::Vector no = ThreeD::Vector::linear(
				normal1[points.next(i)], this->cosines[rotation],
				normal2[points.next(i)], - this->sines[rotation]
			);
			scalars.push_back(normal1[i].scalar_product(no));
		}
		const double last = scalars.back();
		for (std::size_t index = scalars.size() - 1; index > 0; --index) {
			scalars[index] += scalars[index - 1];
		}
		scalars.front() += last;
		std::size_t best = 0;
		double diff = scalars.front();
		for (std::size_t rotation = 1; rotation < scalars.size(); ++rotation) {
			if (scalars[rotation] > diff) {
				diff = scalars[rotation];
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

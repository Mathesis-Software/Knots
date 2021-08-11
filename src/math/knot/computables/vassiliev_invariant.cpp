#include "computables.h"
#include "../Knot.h"

namespace KE { namespace ThreeD { namespace Computables {

VassilievInvariant::VassilievInvariant(const Knot &knot, int order) :
	Computable(knot, "Order " + std::to_string(order) + " Vassiliev invariant"),
	order(order) {
}

double VassilievInvariant::compute() {
	double value = 0.0;

	const auto points = this->knot.points();
	// tangent vectors
	std::vector<Vector> tangents;
	for (std::size_t i = 0; i < points.size(); ++i) {
		tangents.push_back(Vector(points[i], points[points.next(i)]));
	}

	// Вычисляем ``гауссовы произведения''.
	std::vector<std::vector<double>> gauss(points.size());
	for (auto &v : gauss) {
		v.insert(v.end(), points.size(), 0.0);
	}
	for (std::size_t i = 0; i < points.size(); ++i) {
		for (std::size_t j = 0; j < i; j++) {
			const Vector chord(
				(points[i].x + points[points.next(i)].x - points[j].x - points[points.next(j)].x) / 2,
				(points[i].y + points[points.next(i)].y - points[j].y - points[points.next(j)].y) / 2,
				(points[i].z + points[points.next(i)].z - points[j].z - points[points.next(j)].z) / 2
			);
			double chord_len = chord.length();
			gauss[i][j] = tangents[i].scalar_product(tangents[j].vector_product(chord)) / (chord_len * chord_len * chord_len);
			gauss[j][i] = gauss[i][j];
		}
	}

	// Вычисляем суммы ``гауссовых произведений''.

	// В gauss_sum[i1][i2] находится сумма ``гауссовых произведений''
	// для всех хорд с началом в i1 и концом от points.next(i1) до i2.
	std::vector<std::vector<double>> gauss_sum(points.size());
	for (auto &v : gauss_sum) {
		v.insert(v.end(), points.size(), 0.0);
	}
	for (std::size_t i = 0; i < points.size(); i++) {
		for (std::size_t j = points.next(i); j != i; j = points.next(j))
			gauss_sum[i][j] = gauss_sum[i][points.prev(j)] + gauss[i][j];
	}

	for (std::size_t i = 0; i < points.size(); i++) {
		double tmp = 0.0;
		for (std::size_t j = points.next(points.next(i)); j != i; j = points.next(j)) {
			tmp += gauss_sum[points.prev(j)][points.prev(i)] -
						 gauss_sum[j][points.prev(points.prev(j))] + gauss_sum[j][i];
			double tmp2 = gauss[i][j];
			for (int o = 1; o < order; o++) {
				tmp2 *= tmp / 16;
			}
			value += tmp2;
		}
	}

	return value / (4 * M_PI * M_PI);
}

}}}

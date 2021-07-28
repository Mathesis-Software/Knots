#include <cmath>
#include <iostream>

#include "knot.h"

namespace {

struct vector {
	double x, y, z;
	vector(double x, double y, double z) : x(x), y(y), z(z) {}

	double length() const { return sqrt(x * x + y * y + z * z); }
};

double det(const vector &v1, const vector &v2, const vector &v3) {
	return
			v1.x * (v2.y * v3.z - v2.z * v3.y)
		+ v1.y * (v2.z * v3.x - v2.x * v3.z)
		+ v1.z * (v2.x * v3.y - v2.y * v3.x);
}

double det(const double *v1, const double *v2, const double *v3) {
	return
			v1[0] * (v2[1] * v3[2] - v2[2] * v3[1])
		+ v1[1] * (v2[2] * v3[0] - v2[0] * v3[2])
		+ v1[2] * (v2[0] * v3[1] - v2[1] * v3[0]);
}

double vector_square(const double *v) {
	return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
}

double scalar_product(const double *v1, const double *v2) {
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

void vector_product(const double *v1, const double *v2, double *m) {
	m[0] = v1[1] * v2[2] - v1[2] * v2[1];
	m[1] = v1[2] * v2[0] - v1[0] * v2[2];
	m[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

}

#define			points			knot.points
#define			next			knot.next
#define			prev			knot.prev

Knot::AverageCrossingNumber::AverageCrossingNumber(const Knot &knot, bool withSign) :
	Computable(knot, withSign ? "Average signed crossing number" : "Average crossing number"),
	withSign(withSign) {
}

double Knot::AverageCrossingNumber::compute() {
	double value = 0.0;

	// tangent vectors
	std::vector<vector> tangents;
	for (std::size_t i = 0; i < points.size(); ++i) {
		tangents.push_back(vector(
			points[next(i)].x - points[i].x,
			points[next(i)].y - points[i].y,
			points[next(i)].z - points[i].z
		));
	}

	for (std::size_t i = 0; i < points.size(); ++i) {
		for (std::size_t j = 0; j < i; ++j) {
			const vector chord(
				(points[i].x + points[next(i)].x - points[j].x - points[next(j)].x) / 2,
				(points[i].y + points[next(i)].y - points[j].y - points[next(j)].y) / 2,
				(points[i].z + points[next(i)].z - points[j].z - points[next(j)].z) / 2
			);
			const double chord_len = chord.length();
			const double summand = det(tangents[i], tangents[j], chord) / (chord_len * chord_len * chord_len);
			if (this->withSign) {
				value += summand;
			} else {
				value += fabs(summand);
			}
		}
	}

	return value / (2 * M_PI);
}

Knot::VassilievInvariant::VassilievInvariant(const Knot &knot, int order) :
	Computable(knot, "Order " + std::to_string(order) + " Vassiliev invariant"),
	order(order) {
}

double Knot::VassilievInvariant::compute() {
	double value = 0.0;

	// tangent vectors
	std::vector<vector> tangents;
	for (std::size_t i = 0; i < points.size(); ++i) {
		tangents.push_back(vector(
			points[next(i)].x - points[i].x,
			points[next(i)].y - points[i].y,
			points[next(i)].z - points[i].z
		));
	}

	// Вычисляем ``гауссовы произведения''.
	double **gauss = new double*[points.size()];

	for (std::size_t i = 0; i < points.size(); ++i) {
		gauss[i] = new double[points.size()];
		gauss[i][i] = 0.0;
		for (std::size_t j = 0; j < i; j++) {
			const vector chord(
				(points[i].x + points[next(i)].x - points[j].x - points[next(j)].x) / 2,
				(points[i].y + points[next(i)].y - points[j].y - points[next(j)].y) / 2,
				(points[i].z + points[next(i)].z - points[j].z - points[next(j)].z) / 2
			);
			double chord_len = chord.length();
			gauss[i][j] = det(tangents[i], tangents[j], chord) / (chord_len * chord_len * chord_len);
			gauss[j][i] = gauss[i][j];
		}
	}

	// Вычисляем суммы ``гауссовых произведений''.

	// В gauss_sum[i1][i2] находится сумма ``гауссовых произведений''
	// для всех хорд с началом в i1 и концом от next (i1) до i2.
	double **gauss_sum = new double*[points.size()];

	for (std::size_t i = 0; i < points.size(); i++) {
		gauss_sum[i] = new double[points.size()];
		gauss_sum[i][i] = 0.0;
		for (std::size_t j = next (i); j != i; j = next (j))
			gauss_sum[i][j] = gauss_sum[i][prev (j)] + gauss[i][j];
	}

	for (std::size_t i = 0; i < points.size(); i++) {
		double tmp = 0.0;
		for (std::size_t j = next (next (i)); j != i; j = next (j)) {
			tmp += gauss_sum[prev (j)][prev (i)] -
						 gauss_sum[j][prev (prev (j))] + gauss_sum[j][i];
			double tmp2 = gauss[i][j];
			for (int o = 1; o < order; o++) {
				tmp2 *= tmp / 16;
			}
			value += tmp2;
		}
	}

	// Удаляем заранее вычисленные вспомогательные значения.
	for (std::size_t i = 0; i < points.size(); i++)
	{
		delete[] gauss[i];
		delete[] gauss_sum[i];
	}
	delete[] gauss;
	delete[] gauss_sum;

	return value / (4 * M_PI * M_PI);
}

//double Knot::prmExperimental::compute() {
//	double value = 0.0;
//
//	std::size_t i1, i2, i3, i4;
//
//	// Вычисляем заранее касательные векторы.
//	double **tangs = new double*[points.size()];
//	for (i1 = 0; i1 < points.size(); i1++)
//	{
//		tangs[i1] = new double[3];
//		tangs[i1][0] = points[next (i1)].x - points[i1].x;
//		tangs[i1][1] = points[next (i1)].y - points[i1].y;
//		tangs[i1][2] = points[next (i1)].z - points[i1].z;
//	}
//
//	// Вычисляем ``гауссовы произведения''.
//	double chord_len;
//	double ***chord = new double**[points.size()];
//	double ***chord2 = new double**[points.size()];
//	double tmp[3], tmp2[3];
//
//	for (i1 = 0; i1 < points.size(); i1++)
//	{
//		chord[i1] = new double*[points.size()];
//		chord2[i1] = new double*[points.size()];
//		for (i2 = 0; i2 < points.size(); i2++)
//		{
//			chord[i1][i2] = new double[3];
//			chord2[i1][i2] = new double[3];
//		}
//
//		for (i2 = 0; i2 < i1; i2++)
//		{
//			chord[i1][i2][0] = ( points[i1].x + points[next (i1)].x -
//										points[i2].x - points[next (i2)].x ) / 2;
//			chord[i1][i2][1] = ( points[i1].y + points[next (i1)].y -
//										points[i2].y - points[next (i2)].y ) / 2;
//			chord[i1][i2][2] = ( points[i1].z + points[next (i1)].z -
//										points[i2].z - points[next (i2)].z ) / 2;
//			chord[i2][i1][0] = - chord[i1][i2][0];
//			chord[i2][i1][1] = - chord[i1][i2][1];
//			chord[i2][i1][2] = - chord[i1][i2][2];
//			chord_len = sqrt (vector_square (chord[i1][i2]));
//			chord_len = 1.0 / (chord_len * chord_len * chord_len);
//			vector_product (tangs[i1], tangs[i2], tmp);
//			vector_product (tmp, chord[i1][i2], tmp2);
//			vector_product (tmp2, chord[i1][i2], chord2[i1][i2]);
//			chord2[i1][i2][0] *= chord_len;
//			chord2[i1][i2][1] *= chord_len;
//			chord2[i1][i2][2] *= chord_len;
//			chord2[i2][i1][0] = - chord2[i1][i2][0];
//			chord2[i2][i1][1] = - chord2[i1][i2][1];
//			chord2[i2][i1][2] = - chord2[i1][i2][2];
//		}
//	}
//
//	double normal[3];
//
//	for (i1 = 0; i1 < points.size(); i1++)
//		for (i2 = i1 + 1; i2 < points.size(); i2++)
//			for (i3 = i2 + 1; i3 < points.size(); i3++)
//				for (i4 = i3 + 1; i4 < points.size(); i4++)
//	{
//					vector_product (chord[i1][i3], chord[i2][i4], normal);
//
//					value +=
//			scalar_product (chord2[i1][i3], chord[i2][i4]) *
//			scalar_product (chord2[i2][i4], chord[i1][i3]) /
//			vector_square (normal);
//				}
//
//	// Удаляем заранее вычисленные вспомогательные значения.
//	for (i1 = 0; i1 < points.size(); i1++)
//	{
//		for (i2 = 0; i2 < points.size(); i2++)
//		{
//			delete[] chord[i1][i2];
//			delete[] chord2[i1][i2];
//		}
//
//		delete[] tangs[i1];
//		delete[] chord[i1];
//		delete[] chord2[i1];
//	}
//	delete[] tangs;
//	delete[] chord;
//	delete[] chord2;
//
//	return value / (4 * M_PI * M_PI);
//}

double Knot::prmExperimental::compute() {
	double value = 0.0;

	std::size_t i1, i2;

	// Вычисляем заранее касательные векторы.
	double **tangs = new double*[points.size()];
	for (i1 = 0; i1 < points.size(); i1++)
	{
		tangs[i1] = new double[3];
		tangs[i1][0] = points[next (i1)].x - points[i1].x;
		tangs[i1][1] = points[next (i1)].y - points[i1].y;
		tangs[i1][2] = points[next (i1)].z - points[i1].z;
	}

	// Вычисляем ``гауссовы произведения''.
	double chord_len;
	double ***chord = new double**[points.size()];
	double ***vector = new double**[points.size()];
	double **sum = new double*[points.size()];

	for (i1 = 0; i1 < points.size(); i1++)
	{
		chord[i1] = new double*[points.size()];
		vector[i1] = new double*[points.size()];
		sum[i1] = new double[points.size()];
		for (i2 = 0; i2 < points.size(); i2++)
		{
			chord[i1][i2] = new double[3];
			vector[i1][i2] = new double[3];
		}

		for (i2 = 0; i2 < i1; i2++)
		{
			chord[i1][i2][0] = ( points[i1].x + points[next (i1)].x -
										points[i2].x - points[next (i2)].x ) / 2;
			chord[i1][i2][1] = ( points[i1].y + points[next (i1)].y -
										points[i2].y - points[next (i2)].y ) / 2;
			chord[i1][i2][2] = ( points[i1].z + points[next (i1)].z -
										points[i2].z - points[next (i2)].z ) / 2;
			chord_len = sqrt (vector_square (chord[i1][i2]));
			chord[i1][i2][0] /= chord_len;
			chord[i1][i2][1] /= chord_len;
			chord[i1][i2][2] /= chord_len;
			chord[i2][i1][0] = - chord[i1][i2][0];
			chord[i2][i1][1] = - chord[i1][i2][1];
			chord[i2][i1][2] = - chord[i1][i2][2];
			vector_product (tangs[i1], tangs[i2], vector[i1][i2]);
			chord_len *= chord_len;
			vector[i1][i2][0] /= chord_len;
			vector[i1][i2][1] /= chord_len;
			vector[i1][i2][2] /= chord_len;
			vector[i2][i1][0] = - vector[i1][i2][0];
			vector[i2][i1][1] = - vector[i1][i2][1];
			vector[i2][i1][2] = - vector[i1][i2][2];
		}
	}

	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			for (i1 = 0; i1 < points.size(); i1++) {
				sum[i1][i1] = 0.0;
				for (i2 = next (i1); i2 != i1; i2 = next (i2)) {
					sum[i1][i2] = sum[i1][prev (i2)] + chord[i1][i2][i] * vector[i1][i2][j];
				}
			}

			for (i1 = 0; i1 < points.size(); i1++) {
				double tmp = 0.0;

				for (i2 = next (next (i1)); i2 != i1; i2 = next (i2)) {
					tmp += sum[prev (i2)][prev (i1)] - sum[prev (i2)][i2] - sum[i2][prev (prev (i2))] + sum[i2][i1];
					value += tmp * vector[i2][i1][j] * chord[i2][i1][i];
				}
			}
		}
	}

	// Удаляем заранее вычисленные вспомогательные значения.
	for (i1 = 0; i1 < points.size(); i1++)
	{
		for (i2 = 0; i2 < points.size(); i2++)
		{
			delete[] chord[i1][i2];
			delete[] vector[i1][i2];
		}

		delete[] tangs[i1];
		delete[] chord[i1];
		delete[] vector[i1];
		delete[] sum[i1];
	}
	delete[] tangs;
	delete[] chord;
	delete[] vector;
	delete[] sum;

	return value / (64 * M_PI);
}

static bool inside(const double *a, const double *b, const double *c, const double *d, const double *x) {
	const bool sign = det(a, b, x) > 0;
	return (det(b, c, x) > 0) == sign && (det(c, d, x) > 0) == sign && (det(d, a, x) > 0) == sign;
}

static int intersected (const double *a,
												const double *b,
			const double *c,
			const double *d)
{
	if (det (a, b, c) * det (a, b, d) > 0)
		return 0;
	if (det (a, c, d) * det (b, c, d) > 0)
		return 0;
	return 1;
}

double Knot::prmSingular::compute() {
	double value = 0.0;

	std::size_t i1, i2, i3, i4;

	// Вычисляем заранее хорды.
	double chord_len;
	double ***chord = new double**[points.size()];

	for (i1 = 0; i1 < points.size(); i1++)
	{
		chord[i1] = new double*[points.size()];
		for (i2 = 0; i2 < points.size(); i2++)
			chord[i1][i2] = new double[3];

		for (i2 = 0; i2 < i1; i2++)
		{
			chord[i1][i2][0] = points[i1].x - points[i2].x;
			chord[i1][i2][1] = points[i1].y - points[i2].y;
			chord[i1][i2][2] = points[i1].z - points[i2].z;
			chord_len = sqrt (vector_square (chord[i1][i2]));
			chord[i1][i2][0] /= chord_len;
			chord[i1][i2][1] /= chord_len;
			chord[i1][i2][2] /= chord_len;
			chord[i2][i1][0] = - chord[i1][i2][0];
			chord[i2][i1][1] = - chord[i1][i2][1];
			chord[i2][i1][2] = - chord[i1][i2][2];
		}
	}

	double curr_cos, min_cos = 1.0;
	for (i1 = 0; i1 < points.size(); i1++)
		for (i2 = next (i1); i2 != prev (i1); i2 = next (i2))
		{
			curr_cos = fabs (scalar_product (chord[i1][i2], chord[i1][next (i2)]));
			if (min_cos > curr_cos)
				min_cos = curr_cos;
		}
	min_cos = 4 * min_cos - 3;

	for (i1 = 0; i1 < points.size() - 3; i1++)
		for (i2 = i1 + 1; i2 < points.size() - 2; i2++)
			for (i3 = i2 + 1; i3 < points.size() - 1; i3++)
				for (i4 = i3 + 1; i4 < points.size(); i4++)
	{
		if (fabs (scalar_product (chord[i1][i3], chord[i2][i4])) < min_cos)
			continue;
//					value += 1;
//		if (intersected (chord[i1][i3],
//										 chord[i1][next (i3)],
//										 chord[i2][i4],
//				 chord[i2][next (i4)])
//						value += 1;
		if (inside (chord[i1][i3],
								chord[i1][next (i3)],
								chord[next (i1)][next (i3)],
					chord[next (i1)][i3],
								chord[i2][i4]))
						value += 1;
		if (inside (chord[i1][i3],
								chord[i1][next (i3)],
								chord[next (i1)][next (i3)],
					chord[next (i1)][i3],
								chord[i2][next (i4)]))
						value += 1;
		if (inside (chord[i1][i3],
								chord[i1][next (i3)],
								chord[next (i1)][next (i3)],
					chord[next (i1)][i3],
								chord[next (i2)][i4]))
						value += 1;
		if (inside (chord[i1][i3],
								chord[i1][next (i3)],
								chord[next (i1)][next (i3)],
					chord[next (i1)][i3],
								chord[next (i2)][next (i4)]))
						value += 1;
		if (inside (chord[i2][i4],
								chord[i2][next (i4)],
								chord[next (i2)][next (i4)],
					chord[next (i2)][i4],
								chord[i1][i3]))
						value += 1;
		if (inside (chord[i2][i4],
								chord[i2][next (i4)],
								chord[next (i2)][next (i4)],
					chord[next (i2)][i4],
								chord[i1][next (i3)]))
						value += 1;
		if (inside (chord[i2][i4],
								chord[i2][next (i4)],
								chord[next (i2)][next (i4)],
					chord[next (i2)][i4],
								chord[next (i1)][i3]))
						value += 1;
		if (inside (chord[i2][i4],
								chord[i2][next (i4)],
								chord[next (i2)][next (i4)],
					chord[next (i2)][i4],
								chord[next (i1)][next (i3)]))
						value += 1;
				}

	// Удаляем заранее вычисленные вспомогательные значения.
	for (i1 = 0; i1 < points.size(); i1++)
	{
		for (i2 = 0; i2 < points.size(); i2++)
			delete[] chord[i1][i2];
		delete[] chord[i1];
	}
	delete[] chord;

	return value;
}

double Knot::prmExperimental2::compute() {
	double value = 0.0;

	std::size_t i1, i2;
	int o;

	// Вычисляем заранее касательные векторы.
	double **tangs = new double*[points.size()];
	for (i1 = 0; i1 < points.size(); i1++)
	{
		tangs[i1] = new double[3];
		tangs[i1][0] = points[next (i1)].x - points[i1].x;
		tangs[i1][1] = points[next (i1)].y - points[i1].y;
		tangs[i1][2] = points[next (i1)].z - points[i1].z;
	}

	// Вычисляем ``гауссовы произведения''.
	double chord[3], chord_len;
	double **gauss = new double*[points.size()];

	for (i1 = 0; i1 < points.size(); i1++)
	{
		gauss[i1] = new double[points.size()];
		gauss[i1][i1] = 0.0;
		for (i2 = 0; i2 < i1; i2++)
		{
			chord[0] = ( points[i1].x + points[next (i1)].x -
										points[i2].x - points[next (i2)].x ) / 2;
			chord[1] = ( points[i1].y + points[next (i1)].y -
										points[i2].y - points[next (i2)].y ) / 2;
			chord[2] = ( points[i1].z + points[next (i1)].z -
										points[i2].z - points[next (i2)].z ) / 2;
			chord_len = sqrt (vector_square (chord));
			gauss[i1][i2] = det (tangs[i1], tangs[i2], chord) /
												(chord_len * chord_len * chord_len);
			gauss[i2][i1] = gauss[i1][i2];
		}
	}

	// Вычисляем суммы ``гауссовых произведений''.

	// В gauss_sum[i1][i2] находится сумма ``гауссовых произведений''
	// для всех хорд с началом в i1 и концом от next (i1) до i2.
	double **gauss_sum = new double*[points.size()];

	for (i1 = 0; i1 < points.size(); i1++)
	{
		gauss_sum[i1] = new double[points.size()];
		gauss_sum[i1][i1] = 0.0;
		for (i2 = next (i1); i2 != i1; i2 = next (i2))
			gauss_sum[i1][i2] = gauss_sum[i1][prev (i2)] + gauss[i1][i2];
	}

	double tmp, tmp2;
	for (i1 = 0; i1 < points.size(); i1++)
	{
		tmp = 0.0;
		for (i2 = next (next (i1)); i2 != i1; i2 = next (i2))
		{
			tmp += gauss_sum[prev (i2)][prev (i1)] -
						 gauss_sum[i2][prev (prev (i2))] + gauss_sum[i2][i1];
			tmp2 = gauss[i1][i2];
			for (o = 1; o < order; o++)
				tmp2 *= tmp / 16;
			value += tmp2;
		}
	}

	// Удаляем заранее вычисленные вспомогательные значения.
	for (i1 = 0; i1 < points.size(); i1++)
	{
		delete[] tangs[i1];
		delete[] gauss[i1];
		delete[] gauss_sum[i1];
	}
	delete[] tangs;
	delete[] gauss;
	delete[] gauss_sum;

	return value / (4 * M_PI * M_PI);
}
